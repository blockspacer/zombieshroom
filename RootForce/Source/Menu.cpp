#include "Menu.h"

#include <External/Include/dirent/dirent.h>

namespace RootForce
{

	RootForce::Menu::Menu( Awesomium::WebView* p_view, RootEngine::GUISystem::DispatcherInterface* p_dispatcher, RootEngine::GameSharedContext p_context )
	{
		m_view = p_view;
		m_context = p_context;

		Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("Menu"));

		if(result.IsObject() && result.ToObject().type() != Awesomium::kJSObjectType_Local)
		{
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Exit"), JSDelegate(this, &Menu::ExitEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Host"), JSDelegate(this, &Menu::HostEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Connect"), JSDelegate(this, &Menu::ConnectEvent));
			p_dispatcher->Bind(result.ToObject(), Awesomium::WSLit("Refresh"), JSDelegate(this, &Menu::RefreshEvent));
		}
		m_view->set_js_method_handler(p_dispatcher);
		m_view->Focus();
	}

	void RootForce::Menu::Hide()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Hide();"), Awesomium::WebString());
		//m_view->PauseRendering(); // Calling this should save some processing power, but it doesn't wait until the view is updated as transparent
		m_view->ReduceMemoryUsage();
	}
	void RootForce::Menu::Unhide()
	{
		m_view->ExecuteJavascript(Awesomium::WSLit("Unhide();"), Awesomium::WebString());
		//m_view->ResumeRendering();
	}
	void RootForce::Menu::AddServer(const std::pair<uint64_t,RootSystems::ServerInfoInternal>& p_serverInfo)
	{
		std::string command = "AddServer(";
		command = command + "'" + p_serverInfo.second.IP + ":" + std::to_string(p_serverInfo.second.Port) + "',";
		command = command + "'" + p_serverInfo.second.Name.C_String() + "',";
		command = command + "'" + p_serverInfo.second.MapFile.C_String() + "',";
		command = command + std::to_string(p_serverInfo.second.NumPlayers) + ",";
		command = command + std::to_string(p_serverInfo.second.MaxPlayers) + ",";
		command = command + std::to_string(p_serverInfo.first/1000) + ",";
		command = command + "'" + (p_serverInfo.second.PasswordProtected?"Yes":"No") + "');";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}

	void RootForce::Menu::LoadDefaults(RootEngine::ConfigManagerInterface* p_configMan, std::string p_workingDir)
	{
		m_workingDir = p_workingDir;
		std::string command = "SetDefaults(";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerName") + "',";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerMapfile") + "',";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerPort")) + ",";
		command = command + "'" + p_configMan->GetConfigValueAsString("ServerPassword") + "',";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerMaxPlayers")) + ",";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerMatchLength")) + ",";
		command = command + std::to_string(p_configMan->GetConfigValueAsInteger("ServerKillVictory")) + ",";

		command += GetMapList();

		command += ");";

		m_view->ExecuteJavascript(Awesomium::WSLit(command.c_str()), Awesomium::WebString());
	}

	MenuEvent::MenuEvent RootForce::Menu::PollEvent()
	{
		MenuEvent::MenuEvent event;
		if(m_event.size() < 1)
		{
			event.type = MenuEvent::EventType::NoEvent;
			return event;
		}
		event = m_event.at(0); 
		m_event.erase(m_event.begin());
		return event;
	}


	void Menu::ExitEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Exit;

		m_event.push_back(event);
	}


	void RootForce::Menu::HostEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Host;
		event.data = p_array;

		m_event.push_back(event);
	}

	void RootForce::Menu::ConnectEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Connect;
		event.data = p_array;

		m_event.push_back(event);
	}


	void Menu::RefreshEvent( Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array )
	{
		MenuEvent::MenuEvent event;
		event.type = MenuEvent::EventType::Refresh;
		event.data = p_array;

		m_event.push_back(event);
	}

	const Awesomium::JSValue& Menu::GetMapListEvent(Awesomium::WebView* p_caller, const Awesomium::JSArray& p_array)
	{
		return Awesomium::JSValue(Awesomium::WSLit(GetMapList().c_str()));
	}

	std::string Menu::GetMapList()
	{
		// Create a list of available maps
		DIR *dir;
		struct dirent *ent;
		std::string command;
		command += "[";
		if ((dir = opendir((m_workingDir+"Assets/Levels/").c_str())) != NULL)
		{
			bool first = true;
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_type != DT_REG) // Not a regular file
					continue;
				std::string fname = ent->d_name;
				std::string extension = "world";
				if(fname.find(extension, (fname.length() - extension.length())) == std::string::npos) // Look for extension
					continue; // Mismatching extension, skip file
				if(!first)
					command += ",";
				else
					first = !first;
				command = command + "'" + fname + "'";
			}
			closedir (dir);
		}
		else
			m_context.m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Could not open level directory");

		command += "]";
		return command;
	}

}

