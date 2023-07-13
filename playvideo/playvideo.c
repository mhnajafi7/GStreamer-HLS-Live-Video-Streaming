#include <gst/gst.h>

int tutorial_main(int argc, char *argv[]) {
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Build the pipeline */			//Edit the filesrc location and playlist-location and also replace your IP & Port
    pipeline = gst_parse_launch(
        "filesrc location=/path/to/test_video.mp4 ! qtdemux name=demux "
        "demux. ! queue ! decodebin ! videoconvert ! clockoverlay ! x264enc tune=zerolatency ! h264parse ! queue ! mux. "
        "demux. ! queue ! decodebin ! audioconvert ! audioresample ! audio/x-raw,channels=2 ! voaacenc bitrate=128000 ! aacparse ! queue ! mux. "
        "mpegtsmux name=mux ! hlssink playlist-root=http://192.168.6.90:8888 playlist-location=play.m3u8 location=/path/to/segment_%05d.ts target-duration=1 max-files=10",
        NULL);


    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Error handling */
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        GError *err = NULL;
        gchar *debug_info = NULL;
        gst_message_parse_error(msg, &err, &debug_info);
        g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error(&err);
        g_free(debug_info);
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

