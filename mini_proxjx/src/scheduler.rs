use crate::process::{ProcessState, PCB};
use std::collections::VecDeque;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Algorithm {
    FCFS,
    SJF,
    RR,
}

pub struct Scheduler {
    processes: VecDeque<PCB>, // stores all processes in order of arrival
    ready_queue: VecDeque<PCB>,
    terminated_processes: Vec<PCB>,
    algorithm: Algorithm,
    time_quantum: u32, // only for rr
    current_tick: u32,
}

impl Scheduler {
    pub fn new(mut processes: Vec<PCB>, algorithm: Algorithm, time_quantum: u32) -> Self {
        processes.sort_by_key(|p| p.arrival_time);
        Self {
            processes: VecDeque::from(processes),
            ready_queue: VecDeque::new(),
            terminated_processes: Vec::new(),
            algorithm,
            time_quantum,
            current_tick: 0,
        }
    }

    pub fn run(&mut self) {
        println!("\nRunning {:?} Simulation...", self.algorithm);

        let mut running_process: Option<PCB> = None;
        let mut q = 0; // time slice/quantum for RR

        while !self.processes.is_empty() 
        || !self.ready_queue.is_empty() 
        || running_process.is_some() {
            while let Some(p) = self.processes.front() {
                if p.arrival_time <= self.current_tick {
                    let mut arrived = self.processes.pop_front().unwrap();
                    println!("[Tick {}] Process {} ARRIVED", self.current_tick, arrived.pid);
                    arrived.state = ProcessState::Ready;
                    self.ready_queue.push_back(arrived);
                } else {
                    break;
                }
            }
            
            // handle the currently running process
            if let Some(mut running) = running_process.take() {
                running.remaining_time -= 1;
                q += 1;

                if running.remaining_time == 0 {
                    println!("[Tick {}] Process {} FINISHED", self.current_tick, running.pid);
                    running.state = ProcessState::Terminated;
                    running.completion_time = self.current_tick + 1;
                    self.terminated_processes.push(running);
                } else if self.algorithm == Algorithm::RR && q == self.time_quantum {
                    // time slice expired for RR
                    println!("[Tick {}] Process {}'s quantum EXPIRED", self.current_tick, running.pid);
                    running.state = ProcessState::Ready;
                    self.ready_queue.push_back(running);
                } else { running_process = Some(running); }
            }

            // increment waiting time for ready queue PCBs.
            for p in self.ready_queue.iter_mut() {
                p.waiting_time += 1;
            }

            // runs new process if CPU is idle
            if running_process.is_none() && !self.ready_queue.is_empty() {
                let next_process_index = self.next_pcb();
                let mut p = self.ready_queue.remove(next_process_index).unwrap();
                
                println!("[Time {}] Process {} DISPATCHED", self.current_tick, p.pid);
                p.state = ProcessState::Running;
                q = 0; // reset quantum for new process
                running_process = Some(p);
            }
            
            self.current_tick += 1;
        }
        self.summarize();
    }

    fn next_pcb(&mut self) -> usize {
        match self.algorithm {
            Algorithm::FCFS | Algorithm::RR => 0,
            Algorithm::SJF => {
                // finds shortest process in ready queue
                self.ready_queue
                    .iter()
                    .enumerate()
                    .min_by_key(|(_, p)| p.burst_time)
                    .map(|(i, _)| i)
                    .unwrap_or(0)
            }
        }
    }

    fn summarize(&mut self) {
        self.terminated_processes.sort_by_key(|p| p.pid);
        let num_processes = self.terminated_processes.len();
        if num_processes == 0 { return; }

        let total_turnaround: u32 = self.terminated_processes.iter().map(|p| p.completion_time - p.arrival_time).sum();
        let total_waiting: u32 = self.terminated_processes.iter().map(|p| p.waiting_time).sum();

        println!("\n{:?} Summary -->", self.algorithm);
        println!("| PID | Arrival | Burst | Completion | Turnaround | Waiting |");
        for p in &self.terminated_processes {
            let turnaround_time = p.completion_time - p.arrival_time;
            println!(
                "| {:3} | {:7} | {:5} | {:10} | {:10} | {:7} |",
                p.pid, p.arrival_time, p.burst_time, p.completion_time, turnaround_time, p.waiting_time
            );
        }
        println!("Avg Turnaround Time: {:.2} ticks", total_turnaround as f32 / num_processes as f32);
        println!("Avg Waiting Time: {:.2} ticks", total_waiting as f32 / num_processes as f32);
    }
}
