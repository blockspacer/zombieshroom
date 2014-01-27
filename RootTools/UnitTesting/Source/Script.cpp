#include <UnitTesting.h>
#include <RootEngine/Script/Include/RootScript.h>

TEST(Script, Glowballs)
{
	g_engineContext.m_resourceManager->LoadScript("Global");
	g_engineContext.m_resourceManager->LoadScript("TestScript");

	g_engineContext.m_script->SetGlobalNumber("TestVariable", 5);

	g_engineContext.m_script->SetFunction("TestScript", "Test");
	g_engineContext.m_script->ExecuteScript();
}