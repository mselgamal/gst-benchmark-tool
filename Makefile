CC=gcc
CFLAGS=`pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-server-1.0`
LIBS=`pkg-config --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
OUTDIR=build
TARGET=$(OUTDIR)/server


all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): server.c
	$(CC) -o $(TARGET) server.c $(CFLAGS) $(LIBS)

run:
	nohup $(TARGET) > ./server.log 2>&1 &

clean:
	rm -rf $(OUTDIR)