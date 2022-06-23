package com.example.demo;

import java.util.List;
import java.util.Optional;

import com.example.data.Team;
import com.example.data.Player;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


@RestController
@RequestMapping("rest")
public class RESTcontroller {
    @Autowired
    TeamService teamService;

    @Autowired
    PlayerService playerService;

    @GetMapping(value = "teams", produces = {MediaType.APPLICATION_JSON_VALUE})
    public List<Team> getTeams()
    {
        return teamService.getAllTeams();
    }

    @GetMapping(value = "players", produces = {MediaType.APPLICATION_JSON_VALUE})
    public List<Player> getPlayers()
    {
        return playerService.getAllPlayers();
    }

    /*@GetMapping(value = "students/{id}", produces = {MediaType.APPLICATION_XML_VALUE})
    public Player getStudent(@PathVariable("id") int id) {
        Optional<Player> op = studentService.getStudent(id);
        if (op.isEmpty())
            return null;
        return op.get();
    }

    @GetMapping(value = "professors/{id}", produces = {MediaType.APPLICATION_XML_VALUE})
    public Team getProfessor(@PathVariable("id") int id) {
        Optional<Team> op = profService.getProfessor(id);
        if (op.isEmpty())
            return null;
        return op.get();
    }*/

    @PostMapping(value = "teams", consumes = {MediaType.APPLICATION_JSON_VALUE})
    public void addTeam(@RequestBody Team t) {
        teamService.addTeam(t);
    }

    @PostMapping(value = "players", consumes = {MediaType.APPLICATION_JSON_VALUE})
    public void addPlayer(@RequestBody Player p) {
        playerService.addPlayer(p);
    }

    @PutMapping(value = "teams/{id}", consumes = {MediaType.APPLICATION_JSON_VALUE, MediaType.APPLICATION_XML_VALUE})
    public void addTeam(@PathVariable("id") int id, @RequestBody Team t) {
        Optional<Team> op = teamService.getTeam(id);
        if (!op.isEmpty()) {
            Team t1 = op.get();
            t1.setName(t.getName());
            //p1.setOffice(p.getOffice());
            teamService.addTeam(t1);
        }
    }

    @PutMapping(value = "players/{id}", consumes = {MediaType.APPLICATION_JSON_VALUE, MediaType.APPLICATION_XML_VALUE})
    public void addPlayer(@PathVariable("id") int id, @RequestBody Player p) {
        System.out.println("PUT called");
        Optional<Player> op = playerService.getPlayer(id);
        if (!op.isEmpty()) {
            Player p1 = op.get();
            p1.setName(p.getName());
            p1.setBirthdate(p.getBirthdate());
            p1.setPosition(p.getPosition());
            p1.setTeam(p.getTeam());
            playerService.addPlayer(op.get());
        }
    }
}
