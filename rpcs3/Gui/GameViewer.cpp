#include "stdafx.h"
#include "GameViewer.h"
#include "Loader/PSF.h"

static const wxString m_class_name = "GameViewer";
GameViewer::GameViewer(wxWindow* parent) : wxListView(parent)
{
	LoadSettings();
	m_columns.Show(this);

	m_path = "/dev_hdd0/game/";

	Connect(GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(GameViewer::DClick));

	Refresh();
}

GameViewer::~GameViewer()
{
	SaveSettings();
}

void GameViewer::DoResize(wxSize size)
{
	SetSize(size);
}

void GameViewer::LoadGames()
{
	vfsDir dir(m_path);
	ConLog.Write("path: %s", m_path.wx_str());
	if(!dir.IsOpened()) return;

	m_games.Clear();

	for(const DirEntryInfo* info = dir.Read(); info; info = dir.Read())
	{
		if(info->flags & DirEntry_TypeDir)
		{
			m_games.Add(info->name);
		}
	}

	//ConLog.Write("path: %s", m_path.wx_str());
	//ConLog.Write("folders count: %d", m_games.GetCount());
}

void GameViewer::LoadPSF()
{
	m_game_data.Clear();
	for(uint i=0; i<m_games.GetCount(); ++i)
	{
		const wxString& path = m_path + m_games[i] + "/PARAM.SFO";
		vfsFile f;
		if(!f.Open(path))
			continue;

		PSFLoader psf(f);
		if(!psf.Load(false)) continue;
		psf.m_info.root = m_games[i];
		m_game_data.Add(new GameInfo(psf.m_info));
	}

	m_columns.Update(m_game_data);
}

void GameViewer::ShowData()
{
	m_columns.ShowData(this);
}

void GameViewer::Refresh()
{
	Emu.GetVFS().Init(m_path);
	LoadGames();
	LoadPSF();
	ShowData();
	Emu.GetVFS().UnMountAll();
}

void GameViewer::SaveSettings()
{
	m_columns.LoadSave(false, m_class_name, this);
}

void GameViewer::LoadSettings()
{
	m_columns.LoadSave(true, m_class_name);
}

void GameViewer::DClick(wxListEvent& event)
{
	long i = GetFirstSelected();
	if(i < 0) return;

	const wxString& path = m_path + m_game_data[i].root;

	Emu.Stop();
	Emu.GetVFS().Init(path);
	wxString local_path;
	if(Emu.GetVFS().GetDevice(path, local_path) && !Emu.BootGame(local_path.ToStdString()))
	{
		ConLog.Error("Boot error: elf not found! [%s]", path.wx_str());
		return;
	}
	Emu.Run();
}
