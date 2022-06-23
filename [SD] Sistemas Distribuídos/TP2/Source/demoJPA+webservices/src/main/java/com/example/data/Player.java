package com.example.data;

import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import javax.xml.bind.annotation.XmlRootElement;

//import java.time.format.DateTimeFormatter;
//import java.util.Date;

@Entity
@XmlRootElement
public class Player {
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    private String name, position;
    private String birthdate;

    @ManyToOne(cascade = CascadeType.ALL)
    private Team team;
    
    public Player() {
    }
    
    public Player(String name, String position, String birthdate) {
        this.name = name;
        this.position = position;

        
        this.birthdate = birthdate;
    }
    
    public int getId() {
        return this.id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public String getPosition() {
        return position;
    }
    
    @XmlElementWrapper(name = "teams")
    @XmlElement(name = "team")
    public Team getTeam() {
        return team;
    }
    
    public void setTeam(Team team) {
        this.team = team;
    }

    public String getBirthdate() {
        return birthdate;
    }

    public void setBirthdate(String birthdate) {
        this.birthdate = birthdate;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public String toString() {
        return this.name + "(id = " + this.id + "). Position: " + this.birthdate + ". Birthdate: " + this.birthdate;
    }
}
