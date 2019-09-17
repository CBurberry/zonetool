#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		ILocalizeEntry::ILocalizeEntry()
		{
		}

		ILocalizeEntry::~ILocalizeEntry()
		{
		}

		void ILocalizeEntry::init(const std::string& name, std::shared_ptr<ZoneMemory>& mem)
		{
			this->m_name = name;
			this->m_asset = DB_FindXAssetHeader(this->type(), this->name().data(), 1).localize;
		}

		void ILocalizeEntry::prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem)
		{
		}

		void ILocalizeEntry::load_depending(IZone* zone)
		{
		}

		std::string ILocalizeEntry::name()
		{
			return this->m_name;
		}

		std::int32_t ILocalizeEntry::type()
		{
			return localize;
		}

		void ILocalizeEntry::write(IZone* zone, std::shared_ptr<ZoneBuffer>& buf)
		{
			auto data = this->m_asset;
			auto dest = buf->write(data);

			sizeof LocalizeEntry;

			buf->push_stream(3);
			START_LOG_STREAM;

			dest->localizedString = buf->write_str(data->localizedString);
			dest->name = buf->write_str(this->name());

			END_LOG_STREAM;
			buf->pop_stream();
		}

		void ILocalizeEntry::dump(LocalizeEntry* asset)
		{
		}
	}
}
