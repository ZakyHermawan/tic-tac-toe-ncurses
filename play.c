#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define ESC 0x1b

//digunakan untuk menggambar grid di awal
size_t centerPrint(size_t y, char* kata){
  int size = strlen(kata);
  int x = (COLS-size)/2;
  mvprintw(y, x, kata);
  return x;
}

//menggambar grid di awal permainan
//mengembalikan posisi awal dibentuknya grid
size_t drawGrid(){
  centerPrint(4,  "############");
  centerPrint(5,  "#|  |  |  |#");
  centerPrint(6,  "#+--+--+--+#");
  centerPrint(7,  "#|  |  |  |#");
  centerPrint(8,  "#+--+--+--+#");
  centerPrint(9,  "#|  |  |  |#");
  size_t simpan = centerPrint(10,"############");

//#|<posisi yang dikembalikan> jadi harus ditambah 2
  return 2 + simpan;
}

//mengubah posisi x kursor sekarang agar dapat mengakses element matriks
size_t xTukar(size_t x){
  switch(x){
    case 71:
      return 0;
      break;
    case 74:
      return 1;
      break;
    case 77:
      return 2;
      break;
  }
}

//mengubah posisi y kursor sekarang agar dapat mengakses element matriks
size_t yTukar(size_t y){
  switch(y){
    case 5:
      return 0;
      break;
    case 7:
      return 1;
      break;
    case 9:
      return 2;
      break;
  }
}

// mengisi matriks
// 'X' diubah menjadi 1
// 'O' diubah menjadi 2
void isiTabel(size_t y, size_t x, size_t turn, size_t matriks[3][3]){
  size_t baris = yTukar(y);
  size_t kolom = xTukar(x);

  matriks[baris][kolom] = turn;
  return;
}

//memeriksa apakah permainan telah berakhir
size_t akhirPermainan(size_t matriks[3][3]){
  size_t same;
  size_t now;

  //fullcheck draw
  same = 2;
  for(int i=0; i<3; ++i){
    for(int j=0; j<3; ++j){
      if(matriks[i][j]==0){
        same = 0;
        i=j=3;
      }
    }
  }
  if(same) return 2;

  //horizontal check
  for(int i=0; i<3; ++i){
    now = matriks[i][0];
    same = 1;
    for(int j=0; j<3; ++j){
      if(matriks[i][j]!=now || !matriks[i][j]){
        same = 0;
        j = 3;
      }
    }
    if(same) return 1;
  }

  //vertical check
  for(int i=0; i<3; ++i){
    now = matriks[0][i];
    same = 1;
    for(int j=0; j<3; ++j){
      if(matriks[j][i]!=now || !matriks[j][i]){
        same = 0;
        j = 3;
      }
    }
    if(same) return 1;
  }

  same = 1;
  //diagonal check
  now = matriks[0][0];
  for(int i=0; i<3; ++i){
    if(matriks[i][i]!=now || !matriks[i][i]){
      same = 0;
    i = 3;
    }
  }
  if(same) return 1;

  same = 1;
  now = matriks[0][2];
  if(matriks[1][1]!=now || !matriks[1][1]) same = 0;
  else if(matriks[2][0]!=now || !matriks[2][0]) same = 0;
  return same;
}

//memeriksa apakah matriks[y][x] sudah terisi baik oleh 'X' maupun 'O' atau belum
//agar antar 'X' maupun 'O' terjadi tumpang-tindih
size_t cekMatriks(size_t y, size_t x, size_t matriks[3][3]){
  size_t baris, kolom;
  baris = yTukar(y);
  kolom = xTukar(x);

  if(matriks[baris][kolom]) return 1;
  return 0;
}

int main(){
  initscr();
  noecho();
  cbreak();

  size_t height, width, start_y, start_x;
  height =  10;
  width = 20;
  start_y = start_x = 10;


  //menggambar header paling atas 
  for(size_t i=0; i<COLS; ++i){
    printw("=");
  }

  
  //menyimpan kondisi tictactoe saat ini
  size_t matriks[3][3];
  for(size_t i=0; i<3; ++i){
    for(size_t j=0; j<3; ++j){
      matriks[i][j] = 0;
    }
  }

  //menyimpan posisi kursor awal
  size_t xnow, ynow;
  xnow = drawGrid();
  ynow = 5;


  move(ynow, xnow);
  //xnow kini dipindahkan ke posisi (0, 0) pada papan

  //'X' => 1
  //'O' => 2
  size_t turn = 1;

  //agar dapat melisten key_*
  keypad(stdscr, TRUE);
  while(1){
    //variabel masukan bertujuan untuk menerjemahkan aktivitas keyboard user
    size_t masukan = getch();

    //respons untuk masukan bertipe panah
    if(masukan == KEY_RIGHT){
      mvprintw(1, 1, "atas");
      xnow += 3;
      if(xnow > 77) xnow -=3;
      move(ynow, xnow);
    }
    else if(masukan == KEY_LEFT){
      xnow -= 3;
      if(xnow < 71) xnow +=3;
      move(ynow, xnow);
    }
    else if(masukan == KEY_UP){
      ynow -= 2;
      if(ynow < 5) ynow +=2;
      move(ynow, xnow);
    }
    else if(masukan == KEY_DOWN){
      ynow += 2;
      if(ynow > 9) ynow -=2;
      move(ynow, xnow);
    }
    
    //hanya merespons masukan bertipe spasi
    if(turn&1){
      if(masukan==' ' && !cekMatriks(ynow, xnow, matriks)){
        mvprintw(ynow, xnow,"X");
        isiTabel(ynow, xnow, turn, matriks);
        turn = 2;
      }
    }
    else{
      if(masukan==' ' && !cekMatriks(ynow, xnow, matriks)){
        mvprintw(ynow, xnow,"O");
        isiTabel(ynow, xnow, turn, matriks);
        turn=1;
      }
    }
    
    //selalu memeriksa apakah permainan berakhir pada akhir loop
    if(akhirPermainan(matriks)==1){
      mvprintw(1, COLS/2-8, "Selamat, Anda Menang");
      goto HELL;
    }
    else if(akhirPermainan(matriks)==2){
      mvprintw(1, COLS/2-8, "Permainan Seri");
      goto HELL;
    }
    //memberikan informasi dan mengupdate posisi kursor saat ini kepada user
    mvprintw(1, 0, "cursor: %d %d", ynow, xnow);
    move(ynow, xnow);
  }

  //menutup jendela sekaligus menandakah bahwa program berakhir
HELL:
  getch();
  endwin();
  return 0;
}
