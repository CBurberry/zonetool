#pragma once

namespace ZoneTool
{
	namespace IW5
	{
		class IPixelShader : public IAsset
		{
		private:
			std::string m_name;
			PixelShader* m_asset;

			PixelShader* parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem, bool preferLocal = false);

		public:
			IPixelShader();
			~IPixelShader();

			void init(const std::string& name, std::shared_ptr<ZoneMemory>& mem) override;
			void prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem) override;
			void load_depending(IZone* zone) override;

			std::string name() override;
			std::int32_t type() override;
			void write(IZone* zone, std::shared_ptr<ZoneBuffer>& buffer) override;

			static void dump(PixelShader* asset);
		};
	}
}
