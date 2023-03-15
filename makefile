###
###「ばんぐほーき」と、そのツール達の為の makefile
###
###					Turbo-Make 用
### 履歴
###
### Rel.1 .. NF Ver.1 "ばんばすたぁ"
###	不完全バージョン。
###
### Rel.2 .. NF Ver.2 "ばんばすたぁ２"
###	機数を3機まで増やす。2面の最後にボスキャラを用意。
###
### Rel.3 .. NF Ver.3 "ばんばすたぁ２'(ﾀﾞｯｼｭ)"
###	大矢君のキャラクターを導入した。
###
### Rel.4 .. 94年2月版 "ばんぐ！ぞーきん" (中西君にだけ配布したバージョン)
###	オープニング画面を製作。
###	キャラクタデータをソース埋込み式から、ファイルロード方式に変更した。
###	トリガーを離すとホーキが帰ってくるように変更した。
###
### Rel.5 .. 94年3月版 "ばんぐ！ぞーきん"
###	自機がちらつかずに、スクロールと共に流されることがなくなった。
###	プログラム終了後にキーボードの入力が効かなくなくなることがなくなった。
###	ゲームスタート時、オーバー時、面クリア時に簡単な音楽が鳴るようになった。
###

all : bang.exe
# pe.exe me.exe sprchk.exe shiftspr.exe

CC=tcc
LK=tcc
TMPFILE=$(TMP)tmpfile

.c.obj :
	$(CC) -c {$< }

.cpp.obj :
	$(CC) -c $<
#
# ばんぐぞーきん ( ばんぐほーき ver.3 )
#

BANG0=bang.obj main.obj game.obj event.obj map.obj sprite.obj joy.obj segajoy.obj music.obj led.obj
BANG1=enemy.obj enemy0.obj enemy1.obj enemy2.obj enemy3.obj loader.obj keystick.obj scnkey.obj
BANG2=video.obj graph.obj writes.obj heapman.obj beep.obj keyboard.obj sleep.obj symbol.obj

bang : bang.exe
bang.exe : $(BANG0) $(BANG1) $(BANG2)
	echo $(BANG0) >  $(TMPFILE)
	echo $(BANG1) >> $(TMPFILE)
	echo $(BANG2) >> $(TMPFILE)
	$(LK) @$(TMPFILE)

enemy.h : sprite.h
enemies.h : enemy.h
bang.obj : sprite.h enemy.h game.h
enemy.obj : sprite.h enemy.h
game.obj : sprite.h enemy.h game.h
loader.obj : sprite.h
main.obj : sprite.h enemy.h game.h
sprite.obj : sprite.h
event.obj : game.h
enemy1.obj : sprite.h enemy.h game.h
enemy2.obj : sprite.h enemy.h game.h
enemy3.obj : sprite.h enemy.h game.h

scnkey.obj : scnkey.asm
	tasm /ml scnkey.asm;
#
# パターンエディタ
#

PE=pe.obj pe0.obj pe1.obj pe2.obj video.obj keyboard.obj mice.obj writes.obj graph.obj palette.obj
pe.exe : $(PE)
	$(LK) $(PE)

#
# マップエディタ
#

ME=me.obj me0.obj me1.obj map.obj video.obj graph.obj keyboard.obj mice.obj writes.obj sleep.obj
me.exe: $(ME)
	$(LK) $(ME)

#
# .SPRファイルビューヤ
#

SPRCHK= sprchk.obj video.obj sprite.obj keyboard.obj
sprchk.exe : $(SPRCHK)
	$(LK) $(SPRCHK)

#
# .SPRファイル原点移動ツール
#

shiftspr.exe : shiftspr.cpp
	tcc shiftspr.cpp

clean :
	for %I in ($(BANG0)) do del %I
	for %I in ($(BANG1)) do del %I
	for %I in ($(BANG2)) do del %I
	for %I in ($(PE)) do del %I
	for %I in ($(ME)) do del %I
	for %I in (bang.exe pe.exe me.exe sprchk.exe shiftspr.exe) do del %I
