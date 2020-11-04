over-the-magic-fence: main.c person.h spell.h func.h
	cc -o over-the-magic-fence main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
