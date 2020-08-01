#include "pch.h"
#include "czConfig.h"
#include "czApi.h"
#include "czJumpHud.h"
#include <sstream>
namespace czConfig
{

	template <typename T>
	void lookupValue(libconfig::Setting& base, const char* name, T& value)
	{
		if (base.exists(name))
			base.lookupValue(name, value);
	}
	void lookupArray(libconfig::Setting& val, const int elementCount, float* arrVal)
	{
		for (int i = 0; i < val.getLength(); i++)
		{
			arrVal[i] = val[i];
		}
		return;
	}

	void loadSettings()
	{
		using namespace std;
		using namespace libconfig;
		Config cfg;
		try
		{
			cfg.readFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			czApi::writeConsole("[error] Configuration file error! Cannot read file " + configName);
			return;
		}
		catch (const ParseException& pex)
		{
			stringstream x;
			x << "[error] Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
			czApi::writeConsole(x.str());
			return;
		}
		Setting& root = cfg.getRoot();
		if (!root.exists("Hud_Velocity"))
			root.add("Hud_Velocity", Setting::Type::TypeGroup);
		Setting& hud = root["Hud_Velocity"];
		lookupValue(hud, "enabled", hud::vel->enabled);
		lookupValue(hud, "opacity", hud::vel->opacity);
		lookupValue(hud, "fontSize", hud::vel->fontSize);

		if (!root.exists("Hud_Position"))
			root.add("Hud_Position", Setting::Type::TypeGroup);
		Setting& phud = root["Hud_Position"];
		lookupValue(phud, "enabled", hud::pos->enabled);
		lookupValue(phud, "opacity", hud::pos->opacity);
		lookupValue(phud, "fontSize", hud::pos->fontSize);
		if (phud.exists("color"))
			lookupArray(phud["color"], 4, (float*)&hud::pos->tColor);
	}

	void saveSettings()
	{
		using namespace std;
		using namespace libconfig;
		Config cfg;
		bool isNew = false;
		//static const char* output_file = "cfg\\czMain.cfg";
		try
		{
			cfg.readFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			isNew = true;
		}
		Setting& root = cfg.getRoot();
		if (!root.exists("Hud_Velocity"))
			root.add("Hud_Velocity", Setting::TypeGroup);
		if (!root.exists("Hud_Position"))
			root.add("Hud_Position", Setting::TypeGroup);


		Setting& v_Hud = root["Hud_Velocity"];
		update_or_add(v_Hud, "enabled", Setting::TypeBoolean, hud::vel->enabled);
		update_or_add(v_Hud, "opacity", Setting::TypeFloat, hud::vel->opacity);
		update_or_add(v_Hud, "fontSize", Setting::TypeFloat, hud::vel->fontSize);


		Setting& p_Hud = root["Hud_Position"];
		update_or_add(p_Hud, "enabled", Setting::TypeBoolean, hud::pos->enabled);
		update_or_add(p_Hud, "opacity", Setting::TypeFloat, hud::pos->opacity);
		update_or_add(p_Hud, "fontSize", Setting::TypeFloat, hud::pos->fontSize);

		if (!p_Hud.exists("color"))
			p_Hud.add("color", Setting::TypeArray);
		update_or_add_array(p_Hud["color"], 4, Setting::TypeFloat, (float*)&hud::pos->tColor);

		try
		{
			cfg.writeFile(configName.c_str());
		}
		catch (const FileIOException& fioex)
		{
			stringstream x;
			x << "[error] I/O error while writing file: " << configName;
			czApi::writeConsole(x.str());
		}
	}

	template <typename n>
	void update_or_add(libconfig::Setting& val, std::string name, libconfig::Setting::Type type, n value)
	{
		using namespace std;
		using namespace libconfig;

		if (!val.exists(name))
			val.add(name, type);

		val[name] = value;
	}

	void update_or_add_array(libconfig::Setting& val, int elementCount, const libconfig::Setting::Type type, float value[])
	{
		using namespace std;
		using namespace libconfig;
		for (int i = 0; i < elementCount; i++)
		{
			if (i > val.getLength() - 1)
				val.add(type);
			val[i] = value[i];
		}
	}

}
