package de.tuc.barcodescanner;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;

import android.os.AsyncTask;

public class PutScanResultTask extends AsyncTask<String, Void, Boolean> {

	private PutScanResultTaskListener mListener;
	
	public void setListener(PutScanResultTaskListener listener) {
		mListener = listener;
	}
	
	@Override
	protected Boolean doInBackground(String... args) {
		try {
			URL url = new URL(Config.BACKEND + "channel/" + args[0] + "/");
		
		    HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();
			connection.setDoOutput(true);
			connection.setRequestMethod("PUT");
			
			OutputStreamWriter out = new OutputStreamWriter(connection.getOutputStream());
			out.write(args[1]);
			out.close();
			
			connection.getInputStream();
			
			return Boolean.TRUE;
		} catch (IOException ex) {
			return Boolean.FALSE;
		}
	}
	
	@Override
	protected void onPostExecute(Boolean result) {
		if (mListener != null) {
			mListener.finished(result.booleanValue());
		}
	}
	
	public static PutScanResultTask create(String channel, String content) {
		return (PutScanResultTask) new PutScanResultTask().execute(channel, content);
	}
	
}
