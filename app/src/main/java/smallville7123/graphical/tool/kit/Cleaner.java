package smallville7123.graphical.tool.kit;

public class Cleaner {
    public interface Cleanable {
        void clean();
    }
    public static Cleanable register(Object o, Runnable r) {
        CleanerReference c = new CleanerReference(
                java.util.Objects.requireNonNull(o), java.util.Objects.requireNonNull(r));
        phantomReferences.add(c);
        return c;
    }
    private static final java.util.Set<smallville7123.graphical.tool.kit.Cleaner.CleanerReference> phantomReferences
                                             = java.util.concurrent.ConcurrentHashMap.newKeySet();
    private static final java.lang.ref.ReferenceQueue<Object> garbageCollectedObjectsQueue
                                              = new java.lang.ref.ReferenceQueue<>();

    static final class CleanerReference extends java.lang.ref.PhantomReference<Object>
                                        implements Cleanable {
        private final Runnable cleaningAction;

        CleanerReference(Object referent, Runnable action) {
            super(referent, garbageCollectedObjectsQueue);
            cleaningAction = action;
        }
        public void clean() {
            if(phantomReferences.remove(this)) {
                super.clear();
                cleaningAction.run();
            }
        }
    }
    public static void deleteOrphanedNativePeerObjects() {
        CleanerReference reference;
        while((reference=(CleanerReference)garbageCollectedObjectsQueue.poll()) != null) {
            reference.clean();
        }
    }
}