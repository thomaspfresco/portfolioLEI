package com.example.demo;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

import com.example.data.Player;

public interface PlayerRepository extends CrudRepository<Player, Integer> {
    @Query("select p from Player p where p.name like %?1")
    public List<Player> findByNameEndsWith(String chars);

    @Query("select p from Player p where p.team.name like %?1")
    public List<Player> findPlayersFromTeam(String chars);
}    