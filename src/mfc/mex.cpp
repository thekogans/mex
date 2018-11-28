#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include "thekogans/mex/3ds/mex/core/Module.h"
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "MainFrame.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct Mex : public CWinApp {
                    virtual BOOL InitInstance () {
                        // FIXME: Add splash screen.
                        Enable3dControls ();
                        SetRegistryKey ("thekogans.net");
                        MainFrame *mainFrame = new MainFrame;
                        assert (mainFrame != 0);
                        // This call bootstraps the entire UI. After Create
                        // returns, core::UI has been primed, and is ready to go.
                        mainFrame->Create (0, "mex");
                        m_pMainWnd = mainFrame;
                        // The environment is now set. Time to load the modules.
                        core::Module::LoadModules (
                        #if defined (THEKOGANS_MEX_CONFIG_Debug)
                            "modulesd.xml"
                        #else // defined (THEKOGANS_MEX_CONFIG_Debug)
                            "modules.xml"
                        #endif // defined (THEKOGANS_MEX_CONFIG_Debug)
                        );
                        core::Module *module = core::Module::GetCurrModule ();
                        if (module != 0) {
                            module->SetFocus ();
                        }
                        m_pMainWnd->ShowWindow (SW_SHOW);
                        m_pMainWnd->UpdateWindow ();
                        return TRUE;
                    }

                    virtual BOOL OnIdle (LONG lCount) {
                        CWinApp::OnIdle (lCount);
                        core::UI::Instance ().menuBarWindow->OnIdle ();
                        core::UI::Instance ().toolBarWindow->OnIdle ();
                        core::UI::Instance ().viewLayoutWindow->OnIdle ();
                        core::UI::Instance ().speedBarWindow->OnIdle ();
                        core::UI::Instance ().consoleWindow->OnIdle ();
                        core::UI::Instance ().statusBarWindow->OnIdle ();
                        return TRUE;
                    }
                } mex;

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
