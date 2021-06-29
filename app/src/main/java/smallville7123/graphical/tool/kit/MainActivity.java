package smallville7123.graphical.tool.kit;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    smallville7123.graphical.tool.kit.DiligentEngineView diligentEngineView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        diligentEngineView = new DiligentEngineView(this);
        setContentView(diligentEngineView);
    }

    @Override
    protected void onPause() {
        diligentEngineView.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        diligentEngineView.onResume();
    }
}