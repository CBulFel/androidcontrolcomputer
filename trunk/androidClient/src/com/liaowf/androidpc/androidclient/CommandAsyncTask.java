package com.liaowf.androidpc.androidclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import android.widget.Toast;

public class CommandAsyncTask extends android.os.AsyncTask<CommandArg,Integer,CommandResultInfo>{

	private AndroidClient androidClient = null;
	
	public CommandAsyncTask(AndroidClient _androidClient){
		this.androidClient = _androidClient;
	}
	
	
	@Override
	protected CommandResultInfo doInBackground(CommandArg... arg0) {
		// ��̨��������		
		CommandArg commandArg = arg0[0];
		return this.sendCommond(commandArg.ip, commandArg.port, commandArg.command);

	}

	@Override
	protected void onCancelled() {
		// ȡ����������
		super.onCancelled();
	}

	@Override
	protected void onPostExecute(CommandResultInfo result) {
		// �����������֮��
		Toast.makeText(androidClient, result.msg, Toast.LENGTH_LONG).show();
		this.androidClient.getIphostnameTxtView().setText(result.msg);
		
	}

	@Override
	protected void onPreExecute() {
		// ��������֮ǰ
		Toast.makeText(androidClient, "���ڷ�������......", Toast.LENGTH_LONG).show();
		this.androidClient.getIphostnameTxtView().setText("���ڷ�������......");

	}

	@Override
	protected void onProgressUpdate(Integer... values) {
		// ���������еĽ���
		super.onProgressUpdate(values);
	}
	
	
	
	private CommandResultInfo sendCommond(String host,int port,String commond) {

		InputStream in = null;
		OutputStream out = null;

		Socket cSocket = null;
		
		CommandResultInfo resInfo = new CommandResultInfo();
		resInfo.rs = false;
		resInfo.msg = "";
		
		
		if(host==null || "".equals(host)){
			resInfo.rs = false;
			resInfo.msg = "IP Ϊ�գ����Ȼ�ȡIP��";
			
			return resInfo;
			
		}
		
 		try {
			cSocket = new Socket(host, port);

			// ���ó�ʱʱ��
			cSocket.setSoTimeout(Config.connection_timeout);
			if (cSocket != null) {
				out = cSocket.getOutputStream();
				in = cSocket.getInputStream();

				String data = commond;
				out.write(data.getBytes());

			}

		} catch (UnknownHostException e) {
			e.printStackTrace();
			resInfo.rs = false;
			resInfo.msg = "����ͳ��ִ���(δ֪����)��"+e.getMessage();
			return resInfo;
		} catch (IOException e) {
			e.printStackTrace();
			resInfo.rs = false;
			resInfo.msg = "����ͳ��ִ���(�������)��"+e.getMessage();
			return resInfo;
		} finally {
			if (cSocket != null) {
				try {
					cSocket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (out != null) {
				try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (in != null) {
				try {
					in.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

		}
		resInfo.msg ="����ͳɹ�";
		resInfo.rs = true;
		
		return resInfo;

	}

}
