// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define BUTTON_PLAY 501 // идентификатор для кнопки "PLAY"
#define BUTTON_SCORES 502 // идентификатор для кнопки "HIGHSCORES"
#define BUTTON_BACK 503 // идентификатор для кнопки "BACK"
#define GAME 504
#define LIST_SCORES 505
#define STATIC_IMAGE1 506
#define STATIC_NUMB 507
#define STATIC_NUM 508
#define BUTTON_PLUS 509
#define BUTTON_MINUS 510
#define BUTTON_MILTIPLY 511
#define BUTTON_DIVIDE 512
#define SET_TEXT_ON_WINDOW 513
#define GAME_OVER 514
#define STATIC_GAME_OVER 515
#define BUT_PL 516
#define STATIC_POINT 517
#define VOLUME_ON 518
#define VOLUME_OFF 519
#define ID_TOOLBAR 520
#define ID_HELP 521
#define ID_ANSWER 522

#define _CRT_SECURE_NO_WARNINGS // без этого не работают большинство функций str

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
#include "resource.h"

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
