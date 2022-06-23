package com.example.data;

//import javax.persistence.CascadeType;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
//import javax.persistence.OneToOne;
import javax.persistence.ManyToOne;
//import javax.xml.bind.annotation.XmlElement;
//import javax.xml.bind.annotation.XmlElementWrapper;

@Entity
public class Event {
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    
    @ManyToOne
    private Match match;
    
    @ManyToOne
    private Player player; //em caso de golos ou cartoes
    
    private String type; //inicio e fim de jogo, golo, cartoes, ...
    private int time; //0-90 + compensacao

    //private Team team;
    
    public Event() {
    }
    
    public Event(Match match, String type, int time) {
        this.match = match;
        this.type = type;
        this.time = time;
    }

    public Event(Match match, Player player, String type, int time) {
        this.match = match;
        this.player = player;
        this.type = type;
        this.time = time;
    }
    
    public int getId() {
        return this.id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public Match getMatch() {
        return match;
    }
    
    public Player getPlayer() {
        return player;
    }

    public String getType() {
        return type;
    }

    public int getTime() {
        return time;
    }

    public void setMatch (Match match) {
        this.match = match;
    }

    public void setPlayer (Player player) {
        this.player = player;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public String toString() {
       return this.type;
    }
}
