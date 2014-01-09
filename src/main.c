#include "global.h"

void active_hosts (void)
{
    int x, y;
    getmaxyx(stdscr, y, x);
    hosts_window = tui_new_shadowed_win(4, x/2-5, y/2, x/2, "Active clients");
    tui_progress_bar(hosts_window->overlay, 0.8);
    top_panel(hosts_window->panel);
    touchwin(panel_window(hosts_window->panel));
    update_panels( );
    doupdate( );
}

void get_interfaces (void)
{
    int x, y;
    getmaxyx(stdscr, y, x);
    if_window = tui_new_shadowed_win(4, 2, y/2+2, x/3, "Interfaces");
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);
    for ( ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next )
    {
        if ( ifa->ifa_addr->sa_family == AF_INET )
        { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr )->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            wprintw(if_window->overlay,"%s: %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if ( ifAddrStruct != NULL )
    {
        freeifaddrs(ifAddrStruct);
    }
    top_panel(if_window->panel);
    touchwin(panel_window(if_window->panel));
    update_panels( );
    doupdate( );
}

int main(void)
{
    LIBXML_TEST_VERSION
    xmlInitParser( );
    xmlKeepBlanksDefault(0);
    //curl_global_init(CURL_GLOBAL_ALL);

    if ( tui_init( ))
    {
        puts("error");
        return 1;
    }
    int32_t user_key = 0;
    wbkgd(stdscr, COLOR_PAIR(1));
    mvwprintw(stdscr,0,5,"DRBL remote console, F12 to exit");
    //PANEL *TOP = main_window->panel;
    get_interfaces();
    active_hosts();
    cursed *but = tui_new_button(if_window->overlay,11,11,"Refresh", 7);
    tui_toggle_button(but);
    do
    {
        switch ( user_key )
        {

        default:

            break;
        }
        //top_panel(TOP);
        //touchwin(panel_window(TOP));
        update_panels( );
        doupdate( );
    }
    while (( user_key = getch( )) != KEY_F(12));
    tui_del_win(if_window);
    //tui_del_win(hosts_window);
    endwin( );
    xmlCleanupParser( );
    //curl_global_cleanup( );
}

