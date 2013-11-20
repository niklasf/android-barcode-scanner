package de.tuc.barcodescanner;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class MainActivity extends Activity {
	
	private String mChannel;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Button scanButton = (Button) findViewById(R.id.scanButton);
        scanButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View button) {
				IntentIntegrator integrator = new IntentIntegrator(MainActivity.this);
		        integrator.initiateScan();
			}
        	
        });
    }
    
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
    	super.onActivityResult(requestCode, resultCode, intent);
    	
    	IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
    	if (result != null && result.getContents() != null) {
    		String content = result.getContents();
    		
    		if (content.startsWith("http://play.google.com/store/apps/details?id=de.tuc.barcodescanner#")) {
    			setContentView(R.layout.activity_main_spinner);
    			
    			mChannel = content.substring(content.indexOf('#') + 1);
    			content = "";
    		}
    		
    		if (mChannel != null) {
	    		PutScanResultTask.create(mChannel, content).setListener(new PutScanResultTaskListener() {
	    			
					@Override
					public void finished(boolean result) {
						IntentIntegrator integrator = new IntentIntegrator(MainActivity.this);
					   	integrator.initiateScan();
					}
	    				
	    		});
    		}
    	}
    }
    
}
