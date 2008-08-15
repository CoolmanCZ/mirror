#ifndef _usvn_usvn_h_
#define _usvn_usvn_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "SlaveProcess.h"

#define LAYOUTFILE <usvn/usvn.lay>
#include <CtrlCore/lay.h>

class SysConsole : public WithConsoleLayout<TopWindow> {
	typedef SysConsole CLASSNAME;
	
	Font font;
	void AddResult(const String& out);

public:
	int  System(const char *s);
	void Perform()	                            { exit.Show(); Execute(); }

	SysConsole();
};

class SvnSel : public WithSvnSelLayout<TopWindow> {
	String url, usr, pwd;
	String folder;
	
	void Load();
	void Go();
	
	typedef SvnSel CLASSNAME;

public:
	String Select(const char *url, const char *user, const char *pwd);
	
	SvnSel();
};

struct Repo {
	String repo;
	String work;
	String user;
	String pswd;

	void Save(String& s);
	void Load(CParser& p);
};

struct SvnWork {
	String working;
	String user;
	String password;
};

class SvnWorks : public WithSvnWorksLayout<TopWindow> {
	void New();
	void Edit();
	void Remove();
	void Checkout();
	void Sync();
	
	FrameRight<Button> dirsel;
	void DirSel(EditField& f);

public:
	void    Clear();
	void    Add(const String& working, const String& user, const String& data);
	void    Load(const String& text);
	String  Save() const;
	
	int     GetCount() const;
	SvnWork operator[](int i) const;

	typedef SvnWorks CLASSNAME;
	
	SvnWorks();
};

String SvnCmd(const char *cmd, const String& user, const String& pwd);
String SvnCmd(const char *cmd, const SvnWork& w);

struct SvnSync : WithSvnSyncLayout<TopWindow> {
	enum {
		MODIFY,
		CONFLICT,
		ADD,
		REMOVE,
		
		REPOSITORY,
		MESSAGE,
	};
	
	Array<Option>     confirm;
	Array<EditString> message;
	
	SvnWorks works;
	
	void SyncList();
	void Setup();

	typedef SvnSync CLASSNAME;

public:
	void Dir(const char *dir);
	void Perform();
	void DoSync();
	
	SvnSync();
};

#endif
