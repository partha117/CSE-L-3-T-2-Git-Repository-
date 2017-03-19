package sample.Util;

import java.io.Serializable;

/**
 * Created by ASUS on 17-Mar-17.
 */
public class DataPacket implements Serializable {
    public String fileName;
    public int startsFrom;
    public int size;
    public byte[] data=new byte[Constant.packetSize];
    public boolean last=false;

    public DataPacket(String fileName, int startsFrom, int size, byte[] data) {
        this.fileName = fileName;
        this.startsFrom = startsFrom;
        this.size = size;
        this.data = data;
    }

    public String getFileName() {
        return fileName;
    }

    public int getStartsFrom() {
        return startsFrom;
    }

    public int getSize() {
        return size;
    }

    public byte[] getData() {
        return data;
    }

    public boolean isLast() {
        return last;
    }

    public void setLast(boolean last) {
        this.last = last;
    }
}
