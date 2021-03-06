package org.racob.com;

public class VarDesc {
    public static final int VARFLAG_NONE = 0;
    public static final int VARFLAG_FREADONLY = 1;
    public static final int VARFLAG_FSOURCE = 2;
    public static final int VARFLAG_FBINDABLE = 4;
    public static final int VARFLAG_FREQUESTEDIT = 8;
    public static final int VARFLAG_FDISPLAYBIND = 16;
    public static final int VARFLAG_FDEFAULTBIND = 32;
    public static final int VARFLAG_FHIDDEN = 64;
    public static final int VARFLAG_FRESTRICTED = 128;
    public static final int VARFLAG_FDEFAULTCOLLELEM = 256;
    public static final int VARFLAG_FUIDEFAULT = 512;
    public static final int VARFLAG_FNONBROWSABLE = 1024;
    public static final int VARFLAG_FREPLACEABLE = 2048;
    public static final int VARFLAG_FIMMEDIATEBIND = 4096;
    public static final int VARFLAG_DEFAULTFILTER = 192;

    public static final int VAR_PERINSTANCE = 0;
    public static final int VAR_STATIC = 1;
    public static final int VAR_CONST = 2;
    public static final int VAR_DISPATCH = 3;

    private final int varkind;
    private final Variant constant;
    private final int memid;
    private final int flags;

    public VarDesc(int memid, Variant constant, int varkind, int flags)
    {
        this.constant = constant;
        this.memid = memid;
        this.varkind = varkind;
        this.flags = flags;
    }

    public Variant getConstant()
    {
        return constant;
    }

    public int getFlags()
    {
        return flags;
    }
    
    public int getVarkind()
    {
        return varkind;
    }

    public int getMemid()
    {
        return memid;
    }
}
