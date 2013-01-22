NAME	=	id_reversi
SRCS	=	main.c				

OBJS	=	$(SRCS:.c=.o)
LDFLAGS	= ` pkg-config --libs gstreamer-interfaces-0.10 gtk+-2.0 gstreamer-0.10`
CFLAGS	= -Wall -W -Werror -g `pkg-config --cflags gstreamer-interfaces-0.10 gtk+-2.0 gstreamer-0.10`

all:	$(NAME)

$(NAME):	$(OBJS)
	gcc -o $(NAME) $(OBJS) $(LDFLAGS) 

clean:
	rm -f $(OBJS)

distclean:	clean
	rm -f $(NAME)

rebuild:	distclean all

debug: CFLAGS += -D___DEBUG -g3
debug: rebuild
