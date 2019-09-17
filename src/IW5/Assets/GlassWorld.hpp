#pragma once

namespace ZoneTool
{
	namespace IW5
	{
		class IGlassWorld : public IAsset
		{
		private:
			std::string m_name;
			GlassWorld* m_asset;

		public:
			IGlassWorld();
			~IGlassWorld();

			static GlassWorld* parse(const std::string& name, std::shared_ptr<ZoneMemory>& mem);

			void init(const std::string& name, std::shared_ptr<ZoneMemory>& mem) override;
			void init(void* asset, std::shared_ptr<ZoneMemory>& mem) override;

			void prepare(std::shared_ptr<ZoneBuffer>& buf, std::shared_ptr<ZoneMemory>& mem) override;
			void load_depending(IZone* zone) override;

			std::string name() override;
			std::int32_t type() override;

			void write(IZone* zone, std::shared_ptr<ZoneBuffer>& buffer) override;

			static void dump(GlassWorld* asset);
		};
	}
}
