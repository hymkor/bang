###
###�u�΂񂮂ف[���v�ƁA���̃c�[���B�ׂ̈� makefile
###
###					Turbo-Make �p
### ����
###
### Rel.1 .. NF Ver.1 "�΂�΂�����"
###	�s���S�o�[�W�����B
###
### Rel.2 .. NF Ver.2 "�΂�΂������Q"
###	�@����3�@�܂ő��₷�B2�ʂ̍Ō�Ƀ{�X�L������p�ӁB
###
### Rel.3 .. NF Ver.3 "�΂�΂������Q'(�ޯ��)"
###	���N�̃L�����N�^�[�𓱓������B
###
### Rel.4 .. 94�N2���� "�΂񂮁I���[����" (�����N�ɂ����z�z�����o�[�W����)
###	�I�[�v�j���O��ʂ𐻍�B
###	�L�����N�^�f�[�^���\�[�X�����ݎ�����A�t�@�C�����[�h�����ɕύX�����B
###	�g���K�[�𗣂��ƃz�[�L���A���Ă���悤�ɕύX�����B
###
### Rel.5 .. 94�N3���� "�΂񂮁I���[����"
###	���@����������ɁA�X�N���[���Ƌ��ɗ�����邱�Ƃ��Ȃ��Ȃ����B
###	�v���O�����I����ɃL�[�{�[�h�̓��͂������Ȃ��Ȃ��Ȃ邱�Ƃ��Ȃ��Ȃ����B
###	�Q�[���X�^�[�g���A�I�[�o�[���A�ʃN���A���ɊȒP�ȉ��y����悤�ɂȂ����B
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
# �΂񂮂��[���� ( �΂񂮂ف[�� ver.3 )
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
# �p�^�[���G�f�B�^
#

PE=pe.obj pe0.obj pe1.obj pe2.obj video.obj keyboard.obj mice.obj writes.obj graph.obj palette.obj
pe.exe : $(PE)
	$(LK) $(PE)

#
# �}�b�v�G�f�B�^
#

ME=me.obj me0.obj me1.obj map.obj video.obj graph.obj keyboard.obj mice.obj writes.obj sleep.obj
me.exe: $(ME)
	$(LK) $(ME)

#
# .SPR�t�@�C���r���[��
#

SPRCHK= sprchk.obj video.obj sprite.obj keyboard.obj
sprchk.exe : $(SPRCHK)
	$(LK) $(SPRCHK)

#
# .SPR�t�@�C�����_�ړ��c�[��
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
