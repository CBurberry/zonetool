#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		IGfxImage::IGfxImage()
		{
		}

		IGfxImage::~IGfxImage()
		{
		}

		std::string IGfxImage::clean_name(const std::string& name)
		{
			auto newName = name;

			for (auto i = 0u; i < name.size(); i++)
			{
				switch (newName[i])
				{
				case '*':
					newName[i] = '_';
					break;
				}
			}

			return newName;
		}

		GfxImage* IGfxImage::parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			auto path = "images\\" + this->clean_name(name) + ".ffimg";

			if (!FileSystem::FileExists(path))
			{
				return nullptr;
			}

			auto fp = FileSystem::FileOpen(path, "rb");
			if (!fp)
			{
				return nullptr;
			}

			ZONETOOL_INFO("Parsing GfxImage \"%s\"...", name.data());

			auto reader = FileSystem::ToReader(fp);

			auto img = mem->Alloc<GfxImage>();
			img->mapType = reader->Read<char>();
			img->semantic = reader->Read<char>();
			img->category = reader->Read<char>();
			img->flags = reader->Read<char>();
			img->cardMemory = reader->Read<int>();
			img->dataLen1 = reader->Read<int>();
			img->dataLen2 = reader->Read<int>();
			img->height = reader->Read<int>();
			img->width = reader->Read<int>();
			img->depth = reader->Read<int>();
			img->loaded = reader->Read<char>();
			img->name = mem->StrDup(reader->ReadString());

			auto loaddef = mem->Alloc<GfxImageLoadDef>();
			loaddef->mipLevels = reader->Read<char>();
			loaddef->flags = reader->Read<char>();
			loaddef->dimensions[0] = reader->Read<int>();
			loaddef->dimensions[1] = reader->Read<int>();
			loaddef->dimensions[2] = reader->Read<int>();
			loaddef->format = reader->Read<int>();
			loaddef->dataSize = reader->Read<int>();

			GfxImageLoadDef* finalLoaddef = nullptr;

			if (loaddef->dataSize > 4)
			{
				finalLoaddef = mem->ManualAlloc<GfxImageLoadDef>(
					sizeof GfxImageLoadDef +
					(loaddef->dataSize - 4));
				memcpy(finalLoaddef, loaddef, sizeof GfxImageLoadDef);
				reader->ReadManual(&finalLoaddef->texture, loaddef->dataSize, 1);
			}
			else
			{
				finalLoaddef = loaddef;
			}


			img->texture = finalLoaddef;

			FileSystem::FileClose(fp);

			return img;
		}

		void IGfxImage::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = this->parse(name, mem);
			this->isMapImage = (this->m_name.size() >= 6)
				                   ? ((this->m_name.substr(0, 6) == "*light" || this->m_name.substr(0, 6) == "*refle" ||
					                      this->m_name == "$outdoor")
					                      ? true
					                      : false)
				                   : false;

			if (!this->m_asset)
			{
				this->m_asset = DB_FindXAssetHeader(this->type(), this->m_name.data(), 1).gfximage;
			}
		}

		void IGfxImage::init(void* asset, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_asset = reinterpret_cast<GfxImage*>(asset);
			this->m_name = this->m_asset->name;
			this->isMapImage = (this->m_name.size() >= 6)
				                   ? ((this->m_name.substr(0, 6) == "*light" || this->m_name.substr(0, 6) == "*refle" ||
					                      this->m_name == "$outdoor")
					                      ? true
					                      : false)
				                   : false;

			auto parsed = this->parse(this->m_name, mem);
			if (parsed)
			{
				this->m_asset = parsed;
			}
		}

		void IGfxImage::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
		}

		void IGfxImage::load_depending(IZone* zone)
		{
		}

		std::string IGfxImage::name()
		{
			return this->m_name;
		}

		std::int32_t IGfxImage::type()
		{
			return image;
		}

		void IGfxImage::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			if (FileSystem::FileExists(this->name() + ".iwi") && !std::experimental::filesystem::exists(
				"main\\iw5_images\\" + this->name() + ".iwi"))
			{
				auto fp = fopen(
					va("main\\%s\\images\\%s.iwi", FileSystem::GetFastFile().data(), this->name().data()).data(), "wb");
				if (fp)
				{
					auto origfp = FileSystem::FileOpen(this->name() + ".iwi", "rb");

					if (origfp)
					{
						auto origsize = FileSystem::FileSize(origfp);
						auto bytes = FileSystem::ReadBytes(origfp, origsize);
						fwrite(&bytes[0], bytes.size(), 1, fp);
						fclose(origfp);
					}

					fclose(fp);
				}
			}
			else
			{
				ZONETOOL_ERROR("Image \"%s\" not found!", this->name().data());
			}

			auto data = this->m_asset;
			auto dest = buf->write(data);

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->name = buf->write_str(this->name());

			// set loaded to false
			dest->loaded = false;

			buf->push_stream(0);
			if (data->texture)
			{
				buf->align(3);

				auto desttext = buf->at<GfxImageLoadDef>();
				buf->write_stream(data->texture, sizeof GfxImageLoadDef - sizeof std::uintptr_t);

				if (isMapImage && desttext->dataSize)
				{
					buf->write_stream(&data->texture->texture, data->texture->dataSize);
				}
				else
				{
					desttext->dataSize = 0;
				}

				ZoneBuffer::ClearPointer(&dest->texture);
			}
			buf->pop_stream();

			END_LOG_STREAM;
			buf->pop_stream();
		}

		// Legacy cancer code
		void fwritestr(FILE* file, const char* str)
		{
			if (!str)
				return;
			while (*str)
			{
				fwrite(str, 1, 1, file);
				str++;
			}
			fwrite(str, 1, 1, file);
		}

		void fwriteint(FILE* file, int value)
		{
			int _val = value;
			fwrite(&_val, 4, 1, file);
		}

		void fwriteuint(FILE* file, unsigned int value)
		{
			unsigned int _val = value;
			fwrite(&_val, 4, 1, file);
		}

		void fwritechar(FILE* file, char c)
		{
			char _val = c;
			fwrite(&_val, 1, 1, file);
		}

		char cleanAssetName[50];

		char* ClearAssetName(char* name, int maxSize = 50)
		{
			int size = strnlen(name, maxSize);
			char* newName = cleanAssetName;
			memset(newName, 0, size + 1);
			strncpy(newName, name, maxSize);
			for (int i = 0; i < size; i++)
			{
				switch (newName[i])
				{
				case '*':
					newName[i] = '_';
					break;
				}
			}
			return newName;
		}

		void IGfxImage::dump(GfxImage* asset)
		{
			if (asset->texture && asset->texture->dataSize)
			{
				char* newName = ClearAssetName((char*)asset->name);
				auto fp = FileSystem::FileOpen("images/"s + newName + ".ffImg"s, "wb");

				if (!fp) return;

#define fwstr(_str) fwritestr(fp, _str)
#define fwint(_int) fwriteint(fp, _int)
#define fwchr(_chr) fwritechar(fp, _chr)
#define frstr() freadstr(fp)
#define frint() freadint(fp)
#define frchr() freadchar(fp)

				// Header
				fwchr(asset->mapType);
				fwchr(asset->semantic);
				fwchr(asset->category);
				fwchr(asset->flags);
				fwint((int)asset->cardMemory);
				fwint(asset->dataLen1);
				fwint(asset->dataLen2);
				fwint(asset->height);
				fwint(asset->width);
				fwint(asset->depth);
				fwstr(asset->name);

				// LoadDef
				fwchr(asset->texture->mipLevels);
				fwchr(asset->texture->flags);
				fwint(asset->texture->dimensions[0]);
				fwint(asset->texture->dimensions[1]);
				fwint(asset->texture->dimensions[2]);
				fwint(asset->texture->format);
				fwint(asset->texture->dataSize);

				fwrite(&asset->texture->texture, 1, asset->texture->dataSize, fp);

				FileSystem::FileClose(fp);
			}
		}
	}
}
