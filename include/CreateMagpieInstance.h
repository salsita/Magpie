/****************************************************************************
 * CreateMagpieInstance.h : Contains the typedef for creating a magpie
 * instance without the need of having Magpie registered.
 * Copyright 2012 Arne Seib.
 * Author: Arne Seib <kontakt@seiberspace.de>
 *
 * Usage (error handling omitted here):
 *
 * IMagpieApplication* pMagpie = NULL;
 * HMODULE hModMagpie = ::LoadLibrary(_T("magpie.dll"));
 * if (hModMagpie)
 * {
 *   fnCreateMagpieInstance CreateMagpieInstance =
 *     (fnCreateMagpieInstance)::GetProcAddress(hModMagpie,
 *                               "CreateMagpieInstance");
 *   if (CreateMagpieInstance)
 *   {
 *     HRESULT hr = CreateMagpieInstance(&pMagpie);
 *     if (FAILED(hr))
 *     {
 *       // handle error
 *     }
 *   }
 * }
 *
 * ... and when you're done:
 *
 * pMagpie->Release();
 * pMagpie = NULL;
 * fnCreateMagpieInstance = NULL;
 * ::FreeLibrary(hModMagpie);
 *
 * Of course you may (and should) better use a safepointer for pMagpie.
 ****************************************************************************/

typedef HRESULT (__stdcall * fnCreateMagpieInstance)(IMagpieApplication**);

