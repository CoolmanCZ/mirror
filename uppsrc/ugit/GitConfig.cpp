#include "GitConfig.h"

using namespace Upp;

GitConfiguration::GitConfiguration()
{
	CtrlLayout(*this, "GIT configuration");

	this->WhenClose = THISBACK(Close);
	close.WhenPush = THISBACK(Close);

	tabs.WhenAction = THISBACK(CheckModify);

	SetRect(0, 0, 780, 580);
	Sizeable().Zoomable();
}

void GitConfiguration::CheckModify()
{
	int tab = tabs.Get();
	for (int i = 0; i < configurations.GetCount(); ++i) {
		if (configurations[i].IsModified()) {
			tabs.Set(i);
			if (PromptYesNo("Save modified data?"))
				configurations[i].SaveChanges();
			else
				configurations[i].ShowConfigGit();
			break;
		}
	}
	tabs.Set(tab);
}

void GitConfiguration::Init(Vector<String>& gitdirs, const String& packagepath)
{
	int tab = -1;
	configurations.Clear();
	tabs.Reset();

	configurations.Add().SetGitDir(GetHomeDirectory());
	for (int i = 0 ; i < gitdirs.GetCount(); ++i) {
		configurations.Add().SetGitDir(gitdirs[i]);
	}

	for (int i = 0 ; i < configurations.GetCount(); ++i) {
		String tabname = GetFileName(configurations[i].GetGitDir());
		if (configurations[i].GetGitDir().Compare(GetHomeDirectory()) == 0)
			tabname = "Global";
		tabs.Add(configurations[i].SizePos(), tabname);
		configurations[i].ShowConfigGit();

		if (GetGitRoot(packagepath).Compare(configurations[i].GetGitDir()) == 0)
			tab = i;
	}

	if (tab > -1 && tab < configurations.GetCount())
		tabs.Set(tab);
	else
		Exclamation(Format("Package '%s' is not the part of the Git repository!", GetFileTitle(packagepath)));
}

GitConfig::GitConfig()
{
	CtrlLayout(*this);

	eoldl = NOT_SET;
	autocrlfdl = NOT_SET;

	gituser.WhenEnter = THISBACK(UserNameChange);
	gitemail.WhenEnter = THISBACK(EmailChange);

	parameters.AddColumn("Parameter");
	parameters.AddColumn("Value");
	parameters.WhenLeftDouble = THISBACK(UpdateParam);
	parameters.WhenBar = THISBACK(ParamsMenuBar);
	parameters.AutoHideSb();
	parameters.NoWantFocus();

	coreeoldl.WhenAction = THISBACK(CoreEolWhenAction);
	coreeoldl.Tip("Sets the line ending type LF, CRLF or use the platform’s native line ending");
	coreeoldl.Add(NOT_SET);
	coreeoldl.Add("lf");
	coreeoldl.Add("crlf");
	coreeoldl.Add("native");
	coreeoldl <<= eoldl;

	coreautocrlfdl.WhenAction = THISBACK(CoreAutoCrlfWhenAction);
	coreautocrlfdl.Tip("false - no automatic conversion, true - convert LF into CRLF, input - convert CRLF to LF");
	coreautocrlfdl.Add(NOT_SET);
	coreautocrlfdl.Add("false");
	coreautocrlfdl.Add("true");
	coreautocrlfdl.Add("input");
	coreautocrlfdl <<= autocrlfdl;

	local = true;

	BackPaint();
}

void GitConfig::Close()
{
	if (IsModified())
		SaveChanges();
}

void GitConfig::ParamsMenuBar(Bar& bar)
{
	bar.Add("Insert new parameter", THISBACK(InsertParam));
	bar.Add("Update the parameter", THISBACK(UpdateParam));
	bar.Add("Delete the parameter", THISBACK(DeleteParam));
}

void GitConfig::GetParamValue(String &parameter, String& value, const String& title)
{
	WithGitConfigParamLayout2<TopWindow> param_layout;
	CtrlLayout(param_layout, title);

	param_layout.WhenClose = param_layout.Rejector(IDCANCEL);
	param_layout.Acceptor(param_layout.ok, IDOK);
	param_layout.Rejector(param_layout.cancel, IDCANCEL);

	param_layout.param1 <<= parameter;
	param_layout.param2 <<= value;

	for (;;) {
		int c = param_layout.RunAppModal();

		if (c == IDOK) {
			parameter = ~param_layout.param1;
			value = ~param_layout.param2;
			break;
		}
		if (c == IDCANCEL) {
			break;
		}
		Ctrl::EventLoop();
	}
}

void GitConfig::InsertParam()
{
	String parameter, value;

	GetParamValue(parameter, value, "Insert new configuration parameter");
	if (!parameter.IsEmpty() && !value.IsEmpty()) {

		SetGitConfig(local, 'A', parameter, value).ProcessCommandOutput();
		ShowConfigGit();
	}
}

void GitConfig::UpdateParam()
{
	int row = parameters.GetCursor();
	if (row < 0)
		return;

	String parameter = parameters.Get(row, 0).ToString();
	String value = parameters.Get(row, 1).ToString();

	GetParamValue(parameter, value, "Update the configuration parameter");
	if (!parameter.IsEmpty() && !value.IsEmpty()) {

		SetGitConfig(local, 'S', parameter, value).ProcessCommandOutput();
		ShowConfigGit();
	}
}

void GitConfig::DeleteParam()
{
	int row = parameters.GetCursor();
	if (row < 0)
		return;

	String parameter = parameters.Get(row, 0).ToString();

	String prompt = Format("Delete the parameter '%s'?", parameter);
	if (PromptOKCancel(prompt)) {
		SetGitConfig(local, 'U', parameter, "").ProcessCommandOutput();
		ShowConfigGit();
	}
}

void GitConfig::UserNameChange()
{
	String value = gituser.GetData().ToString();
	String oldvalue = user;

	char type = GetConfigChangeType(oldvalue, value);
	if (type > 0) {
		user = value;
		SetGitConfig(local, type, USER_NAME, value).ProcessCommandOutput();
	}
	gituser.ClearModify();
}

void GitConfig::EmailChange()
{
	String value = gitemail.GetData().ToString();
	String oldvalue = email;

	char type = GetConfigChangeType(oldvalue, value);
	if (type > 0) {
		email = value;
		SetGitConfig(local, type, USER_EMAIL, value).ProcessCommandOutput();
	}
	gitemail.ClearModify();
}

void GitConfig::CoreEolWhenAction()
{
	String value = coreeoldl.GetData().ToString();

	char type = GetConfigChangeType(value, eoldl);
	if (type > 0) {
		eoldl = value;
		SetGitConfig(false, type, CORE_EOL, value).ProcessCommandOutput();
	}
	coreeoldl.ClearModify();
}

void GitConfig::CoreAutoCrlfWhenAction()
{
	String value = coreautocrlfdl.GetData().ToString();

	char type = GetConfigChangeType(value, autocrlfdl);
	if (type > 0) {
		autocrlfdl = value;
		SetGitConfig(false, type, CORE_AUTOCRLF, value).ProcessCommandOutput();
	}
	coreautocrlfdl.ClearModify();
}

void GitConfig::ShowConfigGit()
{
	if (GetGitDir().IsEmpty())
		return;

	if (GetGitDir().Compare(GetHomeDirectory()) == 0) {
		local = false;
	}

	if (local) {
		labelbox.SetText("Local");
		coreeol.Hide();
		coreeoldl.Hide();
		coreautocrlf.Hide();
		coreautocrlfdl.Hide();
	}

	parameters.Clear();
	Vector<String> data = GetGitConfig(local).GetOutput();
	if (!GetCmdErrorCode() && data.GetCount() > 0) {
		for (int i = 0; i < data.GetCount(); ++i) {
			Vector<String> param = Split(data[i], "=");
			if (data[i].Find("=") < 0) {
				parameters.Add(data[i]);
			} else {
				if (param[0].Find(USER_NAME) == 0) {
					gituser <<= param[1];
					gituser.ClearModify();
					user = param[1];
					continue;
				}
				if (param[0].Find(USER_EMAIL) == 0) {
					gitemail <<= param[1];
					gitemail.ClearModify();
					email = param[1];
					continue;
				}
				if (param[0].Find(CORE_AUTOCRLF) == 0) {
					coreautocrlfdl = param[1];
					coreautocrlfdl.ClearModify();
					autocrlfdl = param[1];
					continue;
				}
				if (param[0].Find(CORE_EOL) == 0) {
					coreeoldl = param[1];
					coreeoldl.ClearModify();
					eoldl = param[1];
					continue;
				}
				parameters.Add(param[0], param[1]);
			}
		}
	}
}

// vim: ts=4
