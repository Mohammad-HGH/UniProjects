from enum import Enum
from queue import PriorityQueue
from dataclasses import dataclass, field
from collections import deque
from math import log
import random


@dataclass(order=True)
class PrioritizedItem:
    priority: float
    item: object = field()


class EventType(Enum):
    ARRIVE = 1
    DEPART = 2


def random_exp(mean):
    return -log(random.uniform(0, 1)) * mean


class Event:
    def __init__(self, type, time):
        self.type = type
        self.time = time


class SingleServerSimulation:

    def __init__(self):
        self.events = PriorityQueue()
        self.users = PriorityQueue()
        self.server_status = 0
        self.number_in_queue = 0
        self.mean_arrival = 1.0
        self.mean_service = 1.0
        self.departure_count = 0
        self.clock = 0.0
        self.time_of_last_event = 0.0
        self.queue_time = 0.0
        self.busy_time = 0.0
        self.service_time = 0.0
        self.response_time = 0.0
        arrive = Event(EventType.ARRIVE, random_exp(self.mean_arrival))
        self.events.put(PrioritizedItem(arrive.time, arrive))

    def add_departure_event(self):
        serv_time = random_exp(self.mean_service)
        depart = Event(EventType.DEPART, self.clock + serv_time)
        self.service_time += serv_time
        self.events.put(PrioritizedItem(depart.time, depart))
        self.server_status = 1
        self.number_in_queue -= 1

    def handle_arrival(self, event):
        self.users.put(PrioritizedItem(event.time, event))
        self.number_in_queue += 1
        if self.server_status == 0:
            self.add_departure_event()
        else:
            self.busy_time += self.clock - self.time_of_last_event

        next_arrive = Event(EventType.ARRIVE, self.clock + random_exp(self.mean_arrival))
        self.events.put(PrioritizedItem(next_arrive.time, next_arrive))
        self.time_of_last_event = self.clock

    def get_wait_time(self):
        return self.busy_time - self.service_time

    def handle_departure(self, event):
        done = self.users.get().item

        if self.number_in_queue > 0:
            self.add_departure_event()
        else:
            self.server_status = 0
        self.busy_time += (self.clock - self.time_of_last_event)
        self.response_time += (self.clock - done.time)
        self.time_of_last_event = self.clock
        self.departure_count += 1

    def report(self):
        print('Average response time: {}',
              self.response_time / self.departure_count if (self.departure_count > 0) else 'N/A')
        print('Total busy time: {}', self.busy_time)
        print('Number of users in queue: {}'.format(self.number_in_queue))
        print('Number of serviced users: {}'.format(self.departure_count))
        print('Server status: {}'.format(self.server_status))


if __name__ == "__main__":

    limit = 500
    simulation = SingleServerSimulation()

    # Config simulation parameters
    simulation.mean_arrival = 1.0
    simulation.mean_service = 1.0

    while simulation.departure_count < limit:
        event = simulation.events.get().item
        simulation.clock = event.time
        if event.type == EventType.ARRIVE:
            simulation.handle_arrival(event)
        else:
            simulation.handle_departure(event)
        simulation.report()
        input()
