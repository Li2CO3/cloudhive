#include "ui.h"
#include "pages/assemblecraftpage.h"
#include "pages/planebattlepage.h"
#include "pages/planebattlepreppage.h"
#include "pages/pvepage.h"
#include "pages/pvepreppage.h"
#include "pages/settingspage.h"
#include "pages/startpage.h"
#include "pages/numcombpage.h"
#include "pages/numcombpreppage.h"

void MainWindow::load_page(PageType type)
{
    clear_all_object();

    switch (type)
    {
    case STARTPAGE:
    {new StartPage(this); }
    break;

    case PVEPAGE:
    {new PvePage(this); }
    break;

    case PVEPREPPAGE:
    {new PvePrepPage(this); }
    break;

    case SETTINGSPAGE:
    {new SettingsPage(this); }

    break;

    case NUMCOMBPAGE:
    {new NumcombPage(this); }
    break;

    case NUMCOMBPREPPAGE:
    {new NumcombPrepPage(this);}
    break;

    case ASSEMBLECRAFTPAGE:
    {new AssembleCraftPage(this);}
    break;

    case PLANEBATTLEPREPPAGE:
    {new PlaneBattlePrepPage(this);}
    break;

    case PLANEBATTLEPAGE:
    {new PlaneBattlePage(this);}
    break;
    default:throw 0;
    }
    page->load();
    //qDebug()<<"pagetype set to="<<type<<"===";
}
