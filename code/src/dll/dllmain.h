/****************************************************************************
 * dllmain.h : Declaration of module class.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 ****************************************************************************/

class CMagpieAtlDllModule : public CAtlDllModuleT< CMagpieAtlDllModule >
{
public :
  DECLARE_LIBID(LIBID_MagpieLib)
  DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAGPIE, "{D2372E10-B4EA-4C5B-9476-F1DB23C42F1D}")
};

extern class CMagpieAtlDllModule _AtlModule;
