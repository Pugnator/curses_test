/**
 * @file   tui.c
 * @Author Lavrentiy Ivanov (ookami@mail.ru)
 * @date   06.09.2013
 * @brief  Brief description of file.
 *
 * Detailed description of file.
 */

#include "global.h"

int32_t tui_init(void)
{
    //Start curses
    initscr( );
    //No buffer for getch()
    cbreak( );
    raw( );
    nonl( );
    //no echo while entering from keyboard
    noecho( );
    //no text cursor
    curs_set(0);
    //Enable special keys
    keypad(stdscr, TRUE);
    //
    mousemask(ALL_MOUSE_EVENTS, NULL);
    //start color mode
    if ( has_colors( ) == FALSE )
    {
        endwin( );
        printf("\nYour terminal does not support color\n");
        return 1;
    }
    start_color( );
    use_default_colors( );
    //set main colors
    init_pair( 1, COLOR_WHITE,   COLOR_BLUE);
    init_pair( 2, COLOR_GREEN,    COLOR_BLACK);
    init_pair( 3, COLOR_MAGENTA,  COLOR_BLUE);
    init_pair( 4, COLOR_BLACK,     COLOR_YELLOW);
    init_pair( 5, COLOR_GREEN,   COLOR_WHITE);
    init_pair( 6, COLOR_BLUE,   COLOR_WHITE);
    init_pair( 7, COLOR_MAGENTA,   COLOR_WHITE);
    init_pair( 8, COLOR_WHITE, COLOR_MAGENTA);
    init_pair( 9, COLOR_CYAN,    COLOR_WHITE);
    init_pair(10, COLOR_YELLOW,  COLOR_BLACK);
    //assume_default_colors(COLOR_BLUE, COLOR_BLACK);

    return 0;
}

void tui_del_win(cursed *win)
{
    del_panel(win->panel);
    free(win);
}

char *tui_make_input(const char *info, int32_t max_len)
{
    cursed *new = malloc (sizeof *new);
    int x, y;
    getmaxyx(stdscr, y, x);
    new->decoration = newwin(4, max_len + 2, y/2 - 2, x/2 - (max_len + 2)/2);
    wbkgd(new->decoration, COLOR_PAIR(2));

    wborder(new->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration, y - 3, x - 2, 2, 1);
    wbkgd(new->overlay, COLOR_PAIR(2));

    new->panel = new_panel(new->decoration);

    FIELD *field[2];
    field[0] = new_field(1, max_len, 0, 0, 0, 0);
    field[1] = NULL;
    FORM  *my_form;
    set_field_type(field[0],TYPE_IPV4);
    set_field_back(field[0], A_UNDERLINE); /* Print a line for the option  */
    field_opts_off(field[0], O_AUTOSKIP); /* Don't go to next field when this */


    /* Create the form and post it */
    my_form = new_form(field);
    set_form_sub(my_form, new->overlay);
    //set_field_buffer(field[0], 0, buf);
    post_form(my_form);
    mvwprintw(new->decoration,1,1, info);
    int32_t user_key=0;
    keypad(new->overlay, TRUE);

    touchwin(panel_window(new->panel));
    update_panels();
    doupdate();

    do
    {
        switch(user_key)
        {
        case 0xD:
            form_driver(my_form, REQ_VALIDATION);
            goto complete;

        default:
            form_driver(my_form, user_key);
            break;
        }
        touchwin(panel_window(new->panel));
        update_panels();
        doupdate();
    } while((user_key = getch()) != KEY_F(12));
    //wprintw(stdscr,field_buffer(field[0], 0));

complete:
    ;
    char *result=0;
    asprintf(&result, "%s", field_buffer(field[0], 0));

    unpost_form(my_form);
    free_field(field[0]);
    free_form(my_form);
    tui_del_win(new);
    return result;
}

int32_t tui_make_menu (const char **choices, size_t choices_num)
{
    cursed *new = malloc (sizeof *new);
    int x, y;
    getmaxyx(stdscr, y, x);
    new->decoration = newwin(choices_num+2, 20, 5, 5);
    wbkgd(new->decoration, COLOR_PAIR(2));
    wborder(new->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration, y - 2, x - 2, 1, 1);
    wbkgd(new->overlay, COLOR_PAIR(2));
    new->panel = new_panel(new->decoration);
    keypad(new->overlay, TRUE);
    ITEM **my_items = (ITEM **)calloc(choices_num, sizeof(ITEM *));

    for(int i = 0; i < choices_num; ++i)
        my_items[i] = new_item(choices[i], "");

    MENU *my_menu = new_menu((ITEM **)my_items);

    set_menu_win(my_menu, new->overlay);
    set_menu_mark(my_menu, " * ");

    set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
    set_menu_back(my_menu, COLOR_PAIR(2));
    set_menu_grey(my_menu, COLOR_PAIR(3));

    post_menu(my_menu);
    int32_t user_key=0;
    int32_t user_selection=0;

    do
    {
        switch(user_key)
        {
        case KEY_MOUSE:
            menu_driver(my_menu, KEY_MOUSE);
            touchwin(panel_window(new->panel));
            update_panels();
            doupdate();
            sleep(1);
            goto mouse_end;
            break;
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        }
        touchwin(panel_window(new->panel));
        update_panels();
        doupdate();
    } while((user_key = wgetch(new->overlay)) != 0xD);

mouse_end:
    user_selection = item_index(current_item(my_menu));
    unpost_menu(my_menu);
    free_menu(my_menu);
    for(int i = 0; i < choices_num; ++i)
        free_item(my_items[i]);

    tui_del_win(new);
    return user_selection;
}

cursed *tui_new_shadowed_win(int sy, int sx, int h, int w, char *label)
{

    cursed *new = malloc (sizeof *new);
    new->background = newwin(h, w+1, sy, sx);
    wbkgd(new->background, COLOR_PAIR(3));
    wattron(new->background, COLOR_PAIR(2));

    for (int i = w*0.1; i < w-1; i++)
        mvwaddch(new->background, h - 1, i, ' ');
    for (int i= h*0.2; i<h; i++)
        mvwprintw(new->background, i, w - 1, "  ");
    wattroff(new->background, COLOR_PAIR(2));

    new->decoration = derwin(new->background,  h-2, w-2, 1, 1);
    wbkgd(new->decoration, COLOR_PAIR(4));

    box(new->decoration, 0, 0);
    int x, y;
    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration,  y-4, x-2, 3, 1);
    wbkgd(new->overlay, COLOR_PAIR(4));
    new->panel = new_panel(new->background);
    tui_win_label(new->decoration, label, 0);

    update_panels();
    doupdate();
    return new;
}

void tui_progress_bar(WINDOW *win, double progress)
{
    int height, width;
    getmaxyx(win, height, width);
    wattron(win, COLOR_PAIR(8));
    for (int i = 0; i < width * progress; i++)
    {
        mvwaddch(win, (height / 2), i, ' ');
    }
    wattroff(win, COLOR_PAIR(8));
    wattron(win, COLOR_PAIR(7));
    for (int i = width * progress; i < width; i++)
    {
        mvwaddch(win, (height / 2), i, ' ');
    }
    wattroff(win, COLOR_PAIR(7));
    wattron(win, A_STANDOUT);
    mvwprintw(win, (height / 2), (width / 2) - 10, "Extraction progress: %.0f%%", progress*100);
    wattroff(win, A_STANDOUT);
}

void tui_toggle_button (cursed *button)
{
    int x, y;
    getbegyx(button->background, y, x);
    hide_panel(button->panel);
    WINDOW *dup_dec = dupwin(button->decoration);
    PANEL *duppan = new_panel(dup_dec);
    move_panel(duppan, y + 2, x + 3);
    update_panels();
    doupdate();
    sleep(1);
    del_panel(duppan);
    show_panel(button->panel);
}

cursed *tui_new_button(WINDOW *bwin, int sy, int sx, const char *label, size_t len)
{
    cursed *new = malloc (sizeof *new);
    int w = len + 4;
    int h = 5;
    new->background = newwin(h, w + 1, sy, sx);
    wbkgd(new->background, COLOR_PAIR(4));
    /* Get start coordinates of underlying window*/
    int shad_x, shad_y;
    getbegyx(bwin, shad_y, shad_x);

    chtype c[64] = {0};
    /* Extract underlying text and copy it to the button's shadow layer */
    for(int i=0; i < h; i++)
    {
        mvwinchnstr(bwin,sy-shad_y + i,sx-shad_x,c, len + 6);
        mvwaddchstr(new->background,i,0, c);
    }
    wattron(new->background, COLOR_PAIR(10));
    for (int i = 2; i < w - 1; i++)
        mvwaddch(new->background, h - 1, i, ' ');
    for (int i= 2; i < h; i++)
        mvwprintw(new->background, i, w - 1, "  ");
    wattroff(new->background, COLOR_PAIR(10));

    new->decoration = derwin(new->background,  h-2, w-2, 1, 1);
    wbkgd(new->decoration, COLOR_PAIR(1));
    box(new->decoration, 0, 0);
    int x, y;
    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration,  y-2, x-2, 1, 1);
    wbkgd(new->overlay, COLOR_PAIR(1));
    new->panel = new_panel(new->background);
    set_panel_userptr(new->panel, bwin);

    wprintw(new->overlay, label);
    update_panels();
    doupdate();
    return new;
}

cursed *tui_new_win(int sy, int sx, int h, int w, char *label, int bckg)
{
    cursed *new = malloc (sizeof *new);
    //create window and its overlay
/*
       new->background = newwin(h, w, sy, sx);

       wattron(new->background,COLOR_PAIR(7));
       for (int i= w*0.1; i<w;i++)
                mvwaddch(new->background, h-1, i, ' ');
        for (int i= h*0.2; i<h;i++)
                mvwaddch(new->background, i, w-1, ' ');
       wattroff(new->background,COLOR_PAIR(7));
 */
    new->decoration = newwin(h, w, sy, sx);
    wbkgd(new->decoration, COLOR_PAIR(bckg));
    //box(new->decoration, 0, 0);
    //wborder(new->decoration, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(new->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    int x, y;
    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration, y - 4, x - 2, 3, 1);
    wbkgd(new->overlay, COLOR_PAIR(bckg));
    //scrollok(new->overlay, TRUE);
    new->panel = new_panel(new->decoration);
    tui_win_label(new->decoration, label, 0);
    update_panels( );
    doupdate( );
    return new;
}

void tui_popup(char *text)
{
    cursed *new = malloc (sizeof *new);
    int x, y;
    getmaxyx(stdscr, y, x);

    new->decoration = newwin(y/4, x/4, y/2 - y/8, x/2 - x/8);
    wbkgd(new->decoration, COLOR_PAIR(10));

    wborder(new->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER,
            ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    getmaxyx(new->decoration, y, x);
    new->overlay = derwin(new->decoration, y - 2, x - 2, 1, 1);
    wbkgd(new->overlay, COLOR_PAIR(10));
    //scrollok(new->overlay, TRUE);
    new->panel = new_panel(new->decoration);
    //attron(COLOR_PAIR(7));
    //tui_win_label(new->decoration, text, 0);
    wprintw(new->overlay,text);
    //attroff(COLOR_PAIR(7));
    touchwin(panel_window(new->panel));
    update_panels( );
    doupdate( );
    getch();
    tui_del_win(new);
}

void tui_move_panel(cursed *win, int ch)
{
    //Get terminal and window parameters
    int begy, begx, maxx, maxy, x, y;
    getbegyx(panel_window(win->panel), begy, begx);
    getmaxyx(panel_window(win->panel), maxy, maxx);
    getmaxyx(      stdscr,             y,    x);
    //Сhecking edge cases
    switch ( ch )
    {
    case KEY_UP:
        if (( begy - 1 ) >= 0 )
            begy--;
        break;
    case KEY_DOWN:
        if ((( begy + 1 ) + maxy ) <= y )
            begy++;
        break;
    case KEY_LEFT:
        if (( begx - 1 ) >= 0 )
            begx--;
        break;
    case KEY_RIGHT:
        if ((( begx + 1 ) + maxx ) <= x )
            begx++;
        break;
    }
    //Move panel
    move_panel(win->panel, begy, begx);
}

void tui_print_centered(WINDOW *win, char *string, int h_offset, int attr_mask)
{
    int height, width;
    int len = strlen(string);
    getmaxyx(win, height, width);
    wattron(win, attr_mask);
    mvwprintw(win, ( height / 2 ) + h_offset, ( width / 2 ) - ( len / 2 ), string);
    wattroff(win, attr_mask);
}

//Set window's lable
void tui_win_label(WINDOW *win, char *string, int attrib)
{
    if ( !string[0] )
        return;
    if ( attrib == 0 )
        attrib = A_NORMAL;
    //Draw borders
    int i = 0, len = 0, j = 0, k = 0, height, width;
    char label[80] = {0};
    getmaxyx(win, height, width);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    char clearw[128] = {' '};
    clearw[width - 1] = 0;
    mvwprintw(win, 1, 1, clearw);

    len = strlen(string);
    if ( len > width )
        i = len - width;
    else
        i = 0;
    for ( j = i; j < len; j++ )
    {
        label[k] = string[j];
        k++;
    }
    label[k + 1] = '\0';
    if ( len > width )
        label[0] = '~';
    wattron(win, attrib);
    mvwprintw(win, 1, ( width - strlen(string)) / 2, "%s", label);
    wattroff(win, attrib);
}

