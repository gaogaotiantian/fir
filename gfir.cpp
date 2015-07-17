#include <gtk/gtk.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "game.h"
#define BoardSize 19
#define LineWidth 1
#define PieceRadius 10
#define PieceGap 4
#define PieceSize (PieceRadius*2+LineWidth)
#define CanvasWidth ((PieceSize+PieceGap) * BoardSize + PieceGap)
Game game;

GtkWidget *combo_box;

typedef enum NodeType NodeType;
struct PieceInfo {
    NodeType type;
    int x;
    int y;
};

typedef struct PieceInfo PieceInfo;

static cairo_surface_t *surface = NULL;

void InitializeGame();
static void DrawPiece(int, int, NodeType);

static void init_cb(GtkWidget *widget,
                    gpointer   data)
{
    InitializeGame();
    if (game.blackAI.id != 10) {
        Point p;
        p = game.blackAI.func(game.board, Black);
        game.Move(p);
        DrawPiece(p.x, p.y, Black);
    }
    GtkWidget* draw_area = (GtkWidget*) data;
    gtk_widget_queue_draw_area(draw_area, 0, 0, CanvasWidth, CanvasWidth);
}

static void init_rs_cb(GtkWidget *widget,
                       gpointer   data)
{
    InitializeGame();
    int x = 0;
    int y = 0;
    NodeType t;
    for (int i = 0; i <= 2; ++i) {
        t = (i == 1) ? Black : White;
        x = (BoardSize - 9) / 2 + rand() % 9;
        y = (BoardSize - 9) / 2 + rand() % 9;
        if (game.board[x][y] == Empty) {
            game.board[x][y] = t;
            DrawPiece(x, y, t);
        }
    }

    if (game.blackAI.id != 10) {
        Point p;
        p = game.blackAI.func(game.board, Black);
        game.Move(p);
        DrawPiece(p.x, p.y, Black);
    }
    GtkWidget* draw_area = (GtkWidget*) data;
    gtk_widget_queue_draw_area(draw_area, 0, 0, CanvasWidth, CanvasWidth);

}
static void DrawBoard()
{
    cairo_t *cr;
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 0.84, 0.69, 0.57);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, LineWidth);
    for (int i = 0; i < BoardSize; ++i) {
        cairo_move_to(cr, PieceGap + PieceRadius + i * (PieceSize + PieceGap), PieceGap + PieceRadius);
        cairo_line_to(cr, PieceGap + PieceRadius + i * (PieceSize + PieceGap), CanvasWidth - PieceGap - PieceRadius);
        cairo_move_to(cr, PieceGap + PieceRadius, PieceGap + PieceRadius + i * (PieceSize + PieceGap));
        cairo_line_to(cr, CanvasWidth - PieceGap - PieceRadius, PieceGap + PieceRadius + i * (PieceSize + PieceGap));
    }
    cairo_stroke(cr);
    cairo_destroy(cr);
}
static gboolean configure_event_cb (GtkWidget *widget,
                                    GdkEventConfigure *event,
                                    gpointer data)
{
    if (surface)
        cairo_surface_destroy(surface);
    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
                                                CAIRO_CONTENT_COLOR,
                                                CanvasWidth,
                                                CanvasWidth);
    DrawBoard();
    gtk_widget_queue_draw_area(widget, 0, 0, CanvasWidth, CanvasWidth);
    return TRUE;
}

static void draw_cb(GtkWidget *widget, 
                    cairo_t   *cr,
                    gpointer   data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
}
static inline int GetRowColPixel(int x) {
    return PieceGap + PieceRadius + x * (PieceSize + PieceGap);
}
static inline int GetRowColFromPixel(int x) {
    return (int)round((double)(x - PieceGap - PieceRadius) / (PieceSize + PieceGap));
}
static void DrawPiece(int x, int y, NodeType t)
{
    cairo_t *cr;
    //cairo_set_source_surface(cr, surface, 0, 0);
    cr = cairo_create(surface);
    if (t == Black)
        cairo_set_source_rgb(cr, 0, 0, 0);
    else if (t == White)
        cairo_set_source_rgb(cr, 1, 1, 1);
    else
        assert(0);
    cairo_arc(cr, GetRowColPixel(x), GetRowColPixel(y), PieceRadius, 0, 2*G_PI);
    cairo_fill(cr);
    cairo_stroke(cr);
    cairo_destroy(cr);
}
static void PutPiece(GtkWidget *widget, 
                     GdkEventMotion *event, 
                     gpointer   data)
{
    if (game.winner == Empty && game.isBlackPlaying == (game.blackAI.id == 10)) {
        Point p(GetRowColFromPixel(event->x), GetRowColFromPixel(event->y));
        if (game.Move(p) == true) {
            DrawPiece(GetRowColFromPixel(event->x), GetRowColFromPixel(event->y), 
                    game.isBlackPlaying ? White : Black);
            gtk_widget_queue_draw_area(widget, 0, 0, CanvasWidth, CanvasWidth);
            if (game.CheckVictory() != Empty) {
                return;
            } else {
                if (game.isBlackPlaying) {
                    p = game.blackAI.func(game.board, Black);
                    game.Move(p);
                    DrawPiece(p.x, p.y, Black);
                } else {
                    p = game.whiteAI.func(game.board, White);
                    game.Move(p);
                    DrawPiece(p.x, p.y, White);
                }
                if (game.CheckVictory() != Empty)
                    return;
            }
        }
    }
}

void InitializeGame()
{
    game.Initialize();
    srand(time(NULL));
    AI_Map::iterator it = game.aiMap.begin();
    for (; it != game.aiMap.end(); ++it) {
        const char* active_AI = (const char *)gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));
        if (active_AI == NULL) {
            game.SetGamerAI(2, 10);
            break;
        }
        if (strcmp(active_AI, it->second.name) == 0) {
            game.SetGamerAI(it->first, 10);
            break;
        }
    }
    if (it == game.aiMap.end())
        game.SetGamerAI(2, 10);
    DrawBoard();
}
void GetAllAIInComboBox(GtkWidget *widget)
{
    AI_Map::iterator it = game.aiMap.begin();
    for (; it != game.aiMap.end(); ++it) {
        int id = it->first;
        char s_id[10];
        if (id != 1 && id != 10) {
            sprintf(s_id, "%d", id);
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(widget), s_id, game.aiMap[id].name);
        }
    }
}
static void activate(GtkApplication* app, 
                     gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_rs;
    GtkWidget *button_box;
    GtkWidget *draw_area;
    GtkWidget *fixed_container;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Five Son Chess");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 900);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
   
    fixed_container = gtk_fixed_new();
    
    // Draw Area
    draw_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(draw_area, CanvasWidth, CanvasWidth);
    g_signal_connect(draw_area, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(draw_area, "configure-event", G_CALLBACK(configure_event_cb), NULL);
    gtk_widget_add_events(draw_area, GDK_BUTTON_PRESS_MASK);

    g_signal_connect(draw_area, "button_press_event", G_CALLBACK(PutPiece), draw_area);
    
    gtk_fixed_put(GTK_FIXED(fixed_container), draw_area, 0, 0);

    // Buttons
    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_fixed_put(GTK_FIXED(fixed_container), button_box, CanvasWidth, 0);


    button = gtk_button_new_with_label("从头再来");
    g_signal_connect(button, "clicked", G_CALLBACK(init_cb), draw_area);
    button_rs = gtk_button_new_with_label("随机开局");
    g_signal_connect(button_rs, "clicked", G_CALLBACK(init_rs_cb), draw_area);

    combo_box = gtk_combo_box_text_new();
    GetAllAIInComboBox(combo_box);

    gtk_container_add(GTK_CONTAINER(button_box), button);
    gtk_container_add(GTK_CONTAINER(button_box), button_rs);
    gtk_container_add(GTK_CONTAINER(button_box), combo_box);
    gtk_container_add(GTK_CONTAINER(window), fixed_container);

    gtk_widget_show_all(window);

    // Game starts here!
    InitializeGame();
}

int main (int argc, char**argv)
{
    gtk_init(&argc, &argv);
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
