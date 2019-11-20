#ifndef _ugit_GitConfig_h_
#define _ugit_GitConfig_h_

#include <ugit/Git.h>

#define NOT_SET         "not set"
#define USER_NAME       "user.name"
#define USER_EMAIL      "user.email"
#define CORE_EOL        "core.eol"
#define CORE_AUTOCRLF   "core.autocrlf"

struct GitConfig : Upp::WithGitConfigLayout<Upp::ParentCtrl>, public Git {
  private:
	Upp::String user;
	Upp::String email;
	bool local;

	Upp::String eoldl;
	Upp::String autocrlfdl;

	void Close();

	void ParamsMenuBar(Upp::Bar& bar);
	void GetParamValue(Upp::String& parameter, Upp::String& value, const Upp::String& title);
	void InsertParam();
	void UpdateParam();
	void DeleteParam();

	void UserNameChange();
	void EmailChange();
	void CoreEolWhenAction();
	void CoreAutoCrlfWhenAction();

	void ToggleLocal();

  public:
	typedef GitConfig CLASSNAME;

	virtual bool IsModified() const { return (gituser.IsModified() | gitemail.IsModified()); }
	void SaveChanges() {
		UserNameChange();
		EmailChange();
	}

	void ShowConfigGit();

	GitConfig();
};

struct GitConfiguration : Upp::WithGitTabsLayout<Upp::TopWindow> {
  private:
	Upp::Array<GitConfig> configurations;

	void CheckModify();
	void Close() {
		CheckModify();
		TopWindow::Close();
	};

  public:
	typedef GitConfiguration CLASSNAME;

	void Init(Upp::Vector<Upp::String>& gitdirs) { Init(gitdirs, Upp::Null); }
	void Init(Upp::Vector<Upp::String>& gitdirs, const Upp::String& packagepath);

	GitConfiguration();
};

#endif

// vim: ts=4
