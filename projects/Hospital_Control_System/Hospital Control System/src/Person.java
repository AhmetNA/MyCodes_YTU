import java.io.Serializable;

public class Person implements Serializable {
    private final String name;
    private final long national_Id;

    public Person(String name, long Id){
        this.name = name;
        this.national_Id = Id;
    }

    public String getName() {
        return name;
    }

    public long getNational_Id() {
        return national_Id;
    }
}
