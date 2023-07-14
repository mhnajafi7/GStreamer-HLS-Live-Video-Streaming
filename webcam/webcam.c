#include <gst/gst.h>

int tutorial_main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Build the pipeline */
    pipeline = gst_parse_launch(
    "v4l2src device=/dev/video0 ! videoconvert ! videoscale ! videorate ! "
    "video/x-raw,format=I420,width=240,height=144,framerate=30/1 ! "
    "videorate ! video/x-raw,format=I420,framerate=30/1 ! clockoverlay ! "
    "x264enc tune=zerolatency ! mpegtsmux ! hlssink playlist-root=http://192.168.6.90:9999 "
    "playlist-location=play.m3u8 location=/path/to/segment_%05d.ts "
    "target-duration=10 max-files=6",
    NULL);


    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        g_error("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
                "variable set for more details.");
    }

    /* Free resources */
    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}

int main(int argc, char *argv[]) {
    return tutorial_main(argc, argv);
}
