package com.example.demo;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

import com.example.data.Event;

public interface EventRepository extends CrudRepository<Event, Integer> {
    @Query("select p from Player p where p.name like %?1")
    public List<Event> findByNameEndsWith(String chars);

    @Query("select e from Event e where e.match.id =:id order by e.time")
    public List<Event> findEventsFromMatch(int id);

    @Query("select e from Event e where e.type like 'Golo' and e.player.name like %?1")
    public List<Event> findGoalsByPlayer(String chars);

    @Query("select e from Event e where e.type like 'Golo' and e.match.id =:id")
    public List<Event> goalsInMatch(int id);
}    