#include "ui.h"
#include "pages/pvepage.h"
#include "pages/pvepreppage.h"
#include "pages/settingspage.h"
#include "pages/startpage.h"

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
    case PVEPREP:
    {new PvePrepPage(this); }
    break;
    case SETTINGS:
    {new SettingsPage(this); }
    default:break;
    }
    page->load();
    //qDebug()<<"pagetype set to="<<type<<"===";
}
