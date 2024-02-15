#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

int main(int argc, char *argv[]) {
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;

    gst_init(&argc, &argv);

    loop = g_main_loop_new(NULL, FALSE);

    // set debug level
    gst_debug_set_default_threshold(GST_LEVEL_INFO);

    // create a server instance
    server = gst_rtsp_server_new();

    // get the mount points for this server, every server has a default object
    mounts = gst_rtsp_server_get_mount_points(server);
    // make a media factory for a test stream. The default media factory can use gst-launch syntax.
    factory = gst_rtsp_media_factory_new();
    
    // this is the default i used from example
    // gst_rtsp_media_factory_set_launch(factory, "( videotestsrc pattern=ball ! x264enc ! rtph264pay name=pay0 pt=96 )");

    // tested on RPI4 HW
    // custom media factory settings to match cctv camera
    // htop results: 
    // CPU - avg is 2.5-2.8 cpy load over 5/15 mins
    // memory -  approx 950MB 
    //gst_rtsp_media_factory_set_launch(factory, "( videotestsrc pattern=ball ! video/x-raw,width=1920,height=1080,framerate=15/1 ! x264enc bitrate=2000 tune=zerolatency speed-preset=ultrafast ! rtph264pay name=pay0 pt=96 )");
    
    // tested on RPI4 reduced res to 720P
    // cpu - avg is 1.26-1.65
    // mem - 876MB
    gst_rtsp_media_factory_set_launch(factory, "( videotestsrc pattern=ball ! video/x-raw,width=1280,height=720,framerate=15/1 ! x264enc bitrate=2000 tune=zerolatency speed-preset=ultrafast ! rtph264pay name=pay0 pt=96 )");
    
    // tested on RPI4 reduced res to 480P
    // cpu - avg is 0.6-1
    // mem - 876MB
    //gst_rtsp_media_factory_set_launch(factory, "( videotestsrc pattern=ball ! video/x-raw,width=720,height=480,framerate=15/1 ! x264enc bitrate=2000 tune=zerolatency speed-preset=ultrafast ! rtph264pay name=pay0 pt=96 )");

    // test with multisrcfile, the file is .mp4
    //gst_rtsp_media_factory_set_launch(factory,
     //   "( multifilesrc location=/home/mando/repos/gst-benchmark-tool/sample_feed.mp4 loop=true ! decodebin ! videoconvert ! "
    //    "videoscale ! x264enc bitrate=2000 "
    //   "! rtph264pay name=pay0 pt=96 )");

    // test with multisrcfile, the file is .mp4
    //gst_rtsp_media_factory_set_launch(factory, "( multifilesrc location=/home/mando/repos/gst-benchmark-tool/sample_feed.mp4 loop=true ! decodebin ! videoconvert ! videoscale ! x264enc bitrate=2000 ! rtph264pay name=pay0 pt=96 )");

    // attach the test factory to the /test url
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

    // don't need the ref to the mounts anymore
    g_object_unref(mounts);

    // attach the server to the default maincontext
    gst_rtsp_server_attach(server, NULL);

    // start serving
    g_print("stream ready at rtsp://172.16.1.7:8554/test\n");
    g_main_loop_run(loop);

    return 0;
}