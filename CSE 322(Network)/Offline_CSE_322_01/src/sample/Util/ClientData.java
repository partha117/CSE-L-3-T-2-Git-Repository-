package sample.Util;

import sample.Network.NetworkUtil;

/**
 * Created by ASUS on 18-Mar-17.
 */
public class ClientData {
    private NetworkUtil networkUtil;
    private String sid;
    int last;

    public ClientData(NetworkUtil networkUtil, String sid, int last) {
        this.networkUtil = networkUtil;
        this.sid = sid;
        this.last = last;
    }
    public void seen(int over)
    {
        last=over;
    }

    public NetworkUtil getNetworkUtil() {
        return networkUtil;
    }

    public void setNetworkUtil(NetworkUtil networkUtil) {
        this.networkUtil = networkUtil;
    }

    public String getSid() {
        return sid;
    }

    public void setSid(String sid) {
        this.sid = sid;
    }

    public int getLast() {
        return last;
    }

    public void setLast(int last) {
        this.last = last;
    }
}
