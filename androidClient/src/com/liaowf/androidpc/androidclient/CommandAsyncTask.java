package com.liaowf.androidpc.androidclient;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.List;

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
		
		
		return this.sendCommond(commandArg.computerInfos, commandArg.port, commandArg.command);

	}

	@Override
	protected void onCancelled() {
		// ȡ����������
		super.onCancelled();
	}

	@Override
	protected void onPostExecute(CommandResultInfo result) {
		// �����������֮��
		Toast.makeText(androidClient, result.msg, Toast.LENGTH_SHORT).show();
		this.androidClient.getStateTxtView().setText(result.msg);
		
	}

	@Override
	protected void onPreExecute() {
		// ��������֮ǰ
		Toast.makeText(androidClient, "���ڸ���ѡ���Ƶ��Է�������......", Toast.LENGTH_SHORT).show();
		this.androidClient.getStateTxtView().setText("���ڸ���ѡ���Ƶ��Է�������......");

	}

	@Override
	protected void onProgressUpdate(Integer... values) {
		// ���������еĽ���
		super.onProgressUpdate(values);
	}
	
	
	
	private CommandResultInfo sendCommond(List<ComputerInfo> cpList,int port,String commond) {

		InputStream in = null;
		OutputStream out = null;

		Socket cSocket = null;
		
		CommandResultInfo resInfo1 = new CommandResultInfo();
		resInfo1.rs = true;
		resInfo1.msg = "";
		
		
		if(cpList==null || (cpList.size()==0)){
			resInfo1.rs = false;
			resInfo1.msg = "û��ѡ����Ƶ��ԣ����Ȼ�ȡ���Ƶ��ԡ�";
			
			return resInfo1;
			
		}
		
		for(int i=0;i<cpList.size();i++){
			
			CommandResultInfo resInfo = new CommandResultInfo();
			resInfo.rs = true;
			resInfo.msg = "";
			
			ComputerInfo c = (ComputerInfo) cpList.get(i);
			
			try {
				cSocket = new Socket(c.IP, port);

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
				resInfo.msg = "����͵����Ƶ��� "+c.IP+"-"+c.hostName+" ���ִ���(δ֪����)��"+e.getMessage()+";";
            
			} catch (IOException e) {
				e.printStackTrace();
				resInfo.rs = false;
				resInfo.msg = "����͵����Ƶ��� "+c.IP+"-"+c.hostName+" ���ִ���(�������)��"+e.getMessage()+";";

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
			
			if(resInfo.rs==true){
				resInfo.msg ="����͵����Ƶ��� "+c.IP+"-"+c.hostName+" �ɹ�;";
				resInfo.rs = true;
			}		
			
			resInfo1.msg+=resInfo.msg;
		}
		
		
		resInfo1.rs = true;
		resInfo1.msg = "����ѡ���Ƶ��Է���������ɡ�ִ�������"+resInfo1.msg;
 		
		return resInfo1;

	}

}
