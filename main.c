#include "gamecomp.h"
#include "fifo.h"

GtkWidget *window;
GtkWidget *actionSurf;


void endProgram(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
    closePipes(potoki);
}

static gboolean isMoved(gpointer data)
{
    gchar wejscie[boardSize*boardSize+2];
    int s = (boardSize * boardSize + 2);
    if (getStringFromPipe(potoki, wejscie, s)) {
        if((wejscie[0] == '1' && oppTurn == '0') || (wejscie[0] == '0' && oppTurn == '1'))
        {
            oppTurn = wejscie[0];
            if(player_id[0] == 'A')
                Player = 1;
            else
                Player = 2;
            moveStep = 1;
            int i = 0;
            int j = 0;
            for (int k = 1; k < boardSize * boardSize + 1; k++) {
                if (j == boardSize) {
                    j = 0;
                    i++;
                    printf("\n");
                }
                gameBoard[i][j] = wejscie[k];
                printf("%c", gameBoard[i][j]);
                j++;
            }
            updateBoard();
        }
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    if(argc == 1 || (argc == 2 && argv[1][0] == 'A'))
    {
        printf("Podałeś za mało argumentów wywołania!!\n");
        return 0;
    }

    if((potoki = initPipes(argc, argv)) == NULL)
        return 0;

    gtk_init(&argc, &argv);

    if (argc == 3 && strcmp(argv[1],"A") == 0)
    {
        opp_id = "B";
        player_id = "A";
        playerTurn = '1';
        oppTurn = '0';
        Player = 1;
        boardSize = 6;
        moveStep = 1;
    }
    else
    {
        player_id = "B";
        opp_id = "A";
        playerTurn = '1';
        oppTurn = '0';
        Player = 2;
        boardSize = 6;
        moveStep = 1;
    }

    gchar header[30];
    sprintf(header, "Pentago! Gracz %c", player_id[0]);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), header);

    gameField = gtk_grid_new();
    actionSurf = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    upButtons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    downButtons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    initRotationButtons();
    createBoard();
    gtk_box_pack_end(GTK_BOX(actionSurf), downButtons, TRUE, TRUE, 1);
    gtk_box_pack_end(GTK_BOX(actionSurf), gameField, TRUE, TRUE, 1);
    gtk_box_pack_end(GTK_BOX(actionSurf), upButtons, TRUE, TRUE, 1);

    gtk_container_add(GTK_CONTAINER(window), actionSurf);

    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(endProgram), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);

    g_timeout_add(150, isMoved, NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
