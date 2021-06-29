package smallville7123.graphical.tool.kit;

public class MotionEventSerializer {
    private static final String TAG = "MotionEventSerializer";
    private final int reserved = 7;
    private final int segment_length = 9;

    private final int maxSupportedPointers;
    private final int dataLength;
    public final float[] data;

    /**
     * @param maxSupportedPointers must be kept equal with JNI Side
     */
    MotionEventSerializer(int maxSupportedPointers) {
        this.maxSupportedPointers = maxSupportedPointers;
        dataLength = reserved + (segment_length * maxSupportedPointers);
        data = new float[dataLength];
        java.util.Arrays.fill(data, 0.0f);
    }

    // THESE MUSE BE KEPT IN SYNC WITH JNI SIDE
    private static final int MOTION_EVENT_ACTIVE = 1;
    private static final int MOTION_EVENT_NOT_ACTIVE = 2;
    private static final int MOTION_EVENT_ACTION_UNKNOWN = -1;
    private static final int MOTION_EVENT_ACTION_DOWN = 1;
    private static final int MOTION_EVENT_ACTION_UP = 2;
    private static final int MOTION_EVENT_ACTION_MOVE = 3;
    private static final int MOTION_EVENT_MOVED = 1;
    private static final int MOTION_EVENT_DID_NOT_MOVE = 2;
    private static final int idx_dataLength = 0;
    private static final int idx_dataReserved = 1;
    private static final int idx_dataSegmentLength = 2;
    private static final int idx_maxSupportedPointers = 3;
    private static final int idx_ptrCount = 4;
    private static final int idx_ptrCurrentIdx = 5;
    private static final int idx_buttonState = 6;

    /**
     * MotionEvent <br>
     * MOTION_EVENT_ACTION_UP and MOTION_EVENT_ACTION_DOWN
     * always happen sequentially <br><br>
     *
     * MotionEvent <br>
     * MOTION_EVENT_ACTION_MOVE can occur at the same time
     * and are merged into single events when possible<br><br>
     *
     * provides basic pointer tracking by marking which pointers
     * are currently active and which pointers have moved<br><br>
     *
     * public functions in jni side:<br>
     * <li>    void acquire(JNIEnv *jenv, jfloatArray motionEventData);
     * <li>    void release(JNIEnv *jenv, jfloatArray motionEventData);
     * <br><br>
     * <li>    int pointerCount();
     * <li>    int currentPointerIndex();
     * <br><br>
     * <li>    bool isPointerActive(int pointerIndex);
     * <li>    bool didPointerMove(int pointerIndex);
     * <br><br>
     * <li>    int getPointerId(int pointerIndex);
     * <li>    int getAction(int pointerIndex);
     * <li>    int getActionIndex();
     * <li>    float getX(int pointerIndex);
     * <li>    float getY(int pointerIndex);
     * <li>    float getPressure(int pointerIndex);
     * <li>    float getSize(int pointerIndex);
     * <li>    int getButtonState();
     */
    public float[] process(android.view.MotionEvent event) {
        int pointerCount = event.getPointerCount();
        int action = event.getActionMasked();
        int pointerIndex = event.getActionIndex();
        int pointerId = event.getPointerId(pointerIndex);

        data[idx_dataLength] = dataLength;
        data[idx_dataReserved] = reserved;
        data[idx_dataSegmentLength] = segment_length;
        data[idx_maxSupportedPointers] = maxSupportedPointers;
        data[idx_ptrCount] = pointerCount;
        data[idx_ptrCurrentIdx] = pointerIndex;
        data[idx_buttonState] = event.getButtonState();

        int idx_pointerId = (pointerIndex*segment_length)+reserved;
        int idx_ptrIndex = (pointerIndex*segment_length)+reserved+1;
        int idx_action = (pointerIndex*segment_length)+reserved+3;
        int idx_active = (pointerIndex*segment_length)+reserved+2;
        data[idx_pointerId] = pointerId;
        data[idx_ptrIndex] = pointerIndex;
        switch (action) {
            case android.view.MotionEvent.ACTION_DOWN:
            case android.view.MotionEvent.ACTION_POINTER_DOWN:
                data[idx_active] = MOTION_EVENT_ACTIVE;
                data[idx_action] = MOTION_EVENT_ACTION_DOWN;
                break;
            case android.view.MotionEvent.ACTION_MOVE:
                break;
            case android.view.MotionEvent.ACTION_UP:
            case android.view.MotionEvent.ACTION_POINTER_UP:
                data[idx_active] = MOTION_EVENT_NOT_ACTIVE;
                data[idx_action] = MOTION_EVENT_ACTION_UP;
                break;
            default:
                data[idx_action] = MOTION_EVENT_ACTION_UNKNOWN;
                break;
        }
        for (int i = 0; i < pointerCount; i++) {
            if (data[(i*segment_length)+reserved+2] == MOTION_EVENT_ACTIVE) {
                int idx_moved = (i*segment_length)+reserved+4;
                int idx_x = (i*segment_length)+reserved+5;
                int idx_y = (i*segment_length)+reserved+6;
                int idx_pressure = (i*segment_length)+reserved+7;
                int idx_size = (i*segment_length)+reserved+8;
                float x = event.getX(i);
                float y = event.getY(i);
                if (data[idx_x] != x || data[idx_y] != y) {
                    data[idx_x] = x;
                    data[idx_y] = y;
                    data[(i*segment_length)+reserved+3] = MOTION_EVENT_ACTION_MOVE;
                    data[idx_moved] = MOTION_EVENT_MOVED;
                } else {
                    data[idx_moved] = MOTION_EVENT_DID_NOT_MOVE;
                }
                data[idx_pressure] = event.getPressure(i);
                data[idx_size] = event.getSize(i);
            }
        }
        return data;
    }
}
