#ifndef _XMLCONFIG_PARSER_HPP_
#define _XMLCONFIG_PARSER_HPP_

#include <string>
#include <sstream>

#include "../libs/pugixml/pugixml.hpp"

#include "../common/logger.hpp"
#include "../common/utils.hpp"
#include "../common/err_def.hpp"

#include "../common/xmlconfig_def.h"
#include "config_manager.hpp"
#include "proc_manager.hpp"

using namespace std;
using namespace pugi;

namespace dpm
{
	class ProcessConfigurationParser
	{
		static bool _ParseRoot(ERR_ENUM& _err, xml_node& _node, ProcessConfigurationStore& _pcs);
		static bool _ParseGroups(ERR_ENUM& _err, xml_node& _node, ProcessConfigurationStore& _pcs);
		static bool _ParseGroup(xml_node& _node, ProcessGroup& _pg);
		static bool _ParseProcess(xml_node& _node, ProcessInfo& _pi);
		static bool _ParseShm(xml_node& _node, ShmInfo& _si);
		static bool _ParseFlags(xml_node& _node, ProcessFlags& _pi);
		static bool _ParseDependency(xml_node& _node, ProcessDependency& _pd);
		static bool _ParseVariables(xml_node& _node, ProcessVariables& _pv);
		static bool _ParseArgs(xml_node& _node, ProcessVariables& _pv);
		static bool _ParseEnvs(xml_node& _node, ProcessVariables& _pv);
	public:
		static void Parse(ERR_ENUM& _err, string _file, ProcessConfigurationStore& _pcs, bool _clear = false);
	};

}

#endif
