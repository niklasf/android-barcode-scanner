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
	
	private boolean mDisabled;
	
	private void setChannel(String channel) {
		if (channel == null || mDisabled) {
			setContentView(R.layout.activity_main);
	        
	        Button scanButton = (Button) findViewById(R.id.scanButton);
	        scanButton.setOnClickListener(new OnClickListener() {
	
				@Override
				public void onClick(View button) {
					mDisabled = false;
					
					IntentIntegrator integrator = new IntentIntegrator(MainActivity.this);
			        integrator.initiateScan();
				}
	        	
	        });
		} else if (channel != null) {
			setContentView(R.layout.activity_main_spinner);
		}
		
		mChannel = channel;
	}
	
	@Override
	protected void onSaveInstanceState(Bundle savedInstanceState) {
		savedInstanceState.putString("mChannel", mChannel);
		savedInstanceState.putBoolean("mDisabled", mDisabled);
	}
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	
    	if (savedInstanceState != null) {
    		mDisabled = savedInstanceState.getBoolean("mDisabled");
    		setChannel(savedInstanceState.getString("mChannel"));
    	} else {
    		setChannel(null);
    	}
    }
    
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
    	super.onActivityResult(requestCode, resultCode, intent);
    	
    	IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
    	if (result != null) {
    		String content = result.getContents();

    		if (content != null) {
    			if (content.startsWith(Config.BACKEND + "BarcodeScanner.apk#")) {
	    			setChannel(content.substring(content.indexOf('#') + 1));
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
    		} else {
    			mDisabled = true;
    			setContentView(R.layout.activity_main);
    		}
    	}
    }
    
}
