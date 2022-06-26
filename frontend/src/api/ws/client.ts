
export type TerminationReason = "locked" | "terminating" | "error"

interface payload {
    type: "init" | "terminating" | "acknowledged"
    contents?: string
}

export default class Client {
  socket: WebSocket;

  constructor(initCallback?: (...args: any)=> void, terminatingCallback?: (...args: any) => void) {
    this.socket = new WebSocket(`ws://localhost:8080/ws/david`);
    this.socket.onmessage = (message) => {
      const data:payload = JSON.parse(message.data) as payload
      console.log(data);
    }
    // handles violent termination
    // this.socket.close = (___, reason) => 
    //   terminatingCallback?(reason! as TerminationReason)
  }

  public emit(key: string, data?: any) {
    this.socket.send(key);
  }

}