#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ProcessState {
    New,
    Ready,
    Running,
    Terminated,
}

#[derive(Debug, Clone)]
pub struct PCB {
    pub pid: u32,
    pub state: ProcessState,

    pub burst_time: u32,
    pub waiting_time: u32,
    pub arrival_time: u32, // in which tick the process is "created"
    pub completion_time: u32,
    pub remaining_time: u32, // how much time is left for the current burst
}

impl PCB {
    pub fn new(pid: u32, burst_time: u32, arrival_time: u32) -> Self {
        PCB {
            pid,
            state: ProcessState::New,
            arrival_time,
            burst_time,
            remaining_time: burst_time,
            completion_time: 0,
            waiting_time: 0,
        }
    }
}