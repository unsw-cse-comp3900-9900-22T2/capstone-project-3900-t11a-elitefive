import GameState from "../../classes/Gamestate";

export type TerminationReason = "locked" | "terminating" | "error"

interface payload {
  // type: "init" | "terminating" | "acknowledged"
  event: "move" 
  contents?: string
  tile?: string;
}

export default class Client {
  socket: WebSocket;
  gamestate: GameState;

  constructor(gameState: GameState, initCallback?: (...args: any)=> void, terminatingCallback?: (...args: any) => void) {
    this.gamestate = gameState;
    this.socket = new WebSocket(`ws://localhost:8080/ws/david`);
    this.socket.onmessage = (message) => {
      const data:payload = JSON.parse(message.data) as payload
      console.log(data);
      switch(data.event) {
        case "move": {
          // set gamestate tile to be red
          const tile = data.tile;
          if(tile) {
            this.gamestate.setState(tile, { color: "red", user: null });
          }
          // console.log(this.gamestate);
          break;
        }
      }
    }
    // handles violent termination
    // this.socket.close = (___, reason) => 
    //   terminatingCallback?(reason! as TerminationReason)
  }

  public emit(key: string, data?: any) {
    this.socket.send(JSON.stringify({
      "event": key,
      "data": data
    }));
  }

}