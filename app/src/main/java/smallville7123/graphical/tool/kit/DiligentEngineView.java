package smallville7123.graphical.tool.kit;

public class DiligentEngineView extends smallville7123.EGLSurfaceView_EGLTextureView.EGLTextureView {


    private smallville7123.graphical.tool.kit.DiligentEngineView.DiligentEngineRenderer renderer;

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     *
     * @param context
     */
    public DiligentEngineView(android.content.Context context) {
        super(context);
    }

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     *
     * @param context
     * @param attrs
     */
    public DiligentEngineView(android.content.Context context, android.util.AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void init() {
        super.init();
        setEGLContextClientVersion(3);
        setRenderer(renderer = new DiligentEngineRenderer());
    }

    MotionEventSerializer jniMotionEvent = new MotionEventSerializer(10);

    @Override
    public boolean onTouchEvent(android.view.MotionEvent event) {
        return renderer.onTouchEvent(renderer.nativeInstance, jniMotionEvent.process(event));
    }

    private class DiligentEngineRenderer implements Renderer {
        Cleaner.Cleanable cleanable;
        native long createNativeInstance();
        native void destroyNativeInstance(long instance);
        native void onEglSetup(long instance, Object classInstance, String name, String signature);
        native void surfaceChanged(long instance, int w, int h);
        native boolean onTouchEvent(long instance, float[] motionEvent);
        native void onDraw(long instance);
        native void onEglTearDown(long instance);
        long nativeInstance;

        DiligentEngineRenderer() {
            System.loadLibrary("native-lib");
            nativeInstance = createNativeInstance();
            cleanable = Cleaner.register(this, () -> destroyNativeInstance(nativeInstance));
        }

        @Override
        public void onEglSetup() {
            onEglSetup(nativeInstance, DiligentEngineView.this,
                    getJavaNameForJNI(METHOD.SWAP_BUFFERS),
                    getJavaSignatureForJNI(METHOD.SWAP_BUFFERS)
            );
        }

        @Override
        public void onEglTearDown() {
            onEglTearDown(nativeInstance);
        }

        @Override
        public void onSurfaceCreated(javax.microedition.khronos.opengles.GL10 gl, javax.microedition.khronos.egl.EGLConfig config) {
            // do nothing
        }

        @Override
        public void onSurfaceChanged(javax.microedition.khronos.opengles.GL10 gl, int width, int height) {
            surfaceChanged(nativeInstance, width, height);
        }

        @Override
        public void onDrawFrame(javax.microedition.khronos.opengles.GL10 gl) {
            onDraw(nativeInstance);
        }
    }
}
