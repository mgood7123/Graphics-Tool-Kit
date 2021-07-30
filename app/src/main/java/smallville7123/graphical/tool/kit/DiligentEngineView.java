package smallville7123.graphical.tool.kit;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import smallville7123.EGLSurfaceView_EGLTextureView.EGLTextureView;

public class DiligentEngineView extends EGLTextureView {


    private DiligentEngineView.DiligentEngineRenderer renderer;

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     *
     * @param context
     */
    public DiligentEngineView(Context context) {
        super(context);
    }

    /**
     * Standard View constructor. In order to render something, you
     * must call {@link #setRenderer} to register a renderer.
     *
     * @param context
     * @param attrs
     */
    public DiligentEngineView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void init() {
        super.init();
        setEGLContextClientVersion(3);
        setRenderer(renderer = new DiligentEngineRenderer());
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getActionMasked();
        int actionIndex = event.getActionIndex();
        int id = 0;
        boolean isUpDown = false;
        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_DOWN:
            case MotionEvent.ACTION_POINTER_UP:
                id = event.getPointerId(actionIndex);
                isUpDown = true;
                break;
        }

        int c = event.getPointerCount();
        for (int i = 0; i < c; i++) {
            int pid = event.getPointerId(i);
            if (isUpDown) {
                if (actionIndex == i) {
                    switch (action) {
                        case MotionEvent.ACTION_DOWN:
                        case MotionEvent.ACTION_POINTER_DOWN:
                            renderer.addTouch(
                                    id,
                                    event.getX(actionIndex),
                                    event.getY(actionIndex),
                                    event.getSize(actionIndex),
                                    event.getPressure(actionIndex)
                            );
                            break;
                        case MotionEvent.ACTION_UP:
                        case MotionEvent.ACTION_POINTER_UP:
                            renderer.removeTouch(
                                    id,
                                    event.getX(actionIndex),
                                    event.getY(actionIndex),
                                    event.getSize(actionIndex),
                                    event.getPressure(actionIndex)
                            );
                            break;
                    }
                }
            }

            if (actionIndex != i || !isUpDown) {
                renderer.moveTouch(
                        pid,
                        event.getX(i),
                        event.getY(i),
                        event.getSize(i),
                        event.getPressure(i)
                );
            }
        }
        return renderer.onTouchEvent(renderer.nativeInstance);
    }

    private class DiligentEngineRenderer implements Renderer {
        Cleaner.Cleanable cleanable;
        native long createNativeInstance();
        native void destroyNativeInstance(long instance);
        native void onEglSetup(long instance, Object classInstance, String name, String signature);
        native void surfaceChanged(long instance, int w, int h);
        native void addTouch(long identity, float x, float y);
        native void addTouch(long identity, float x, float y, float size);
        native void addTouch(long identity, float x, float y, float size, float pressure);
        native void moveTouch(long identity, float x, float y);
        native void moveTouch(long identity, float x, float y, float size);
        native void moveTouch(long identity, float x, float y, float size, float pressure);
        native void removeTouch(long identity, float x, float y);
        native void removeTouch(long identity, float x, float y, float size);
        native void removeTouch(long identity, float x, float y, float size, float pressure);
        native void cancelTouch(long identity, float x, float y);
        native void cancelTouch(long identity, float x, float y, float size);
        native void cancelTouch(long identity, float x, float y, float size, float pressure);
        native boolean onTouchEvent(long instance);
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
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // do nothing
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            surfaceChanged(nativeInstance, width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            onDraw(nativeInstance);
        }
    }
}
