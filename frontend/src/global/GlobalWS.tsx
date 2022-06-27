import React, { useContext, useEffect, useState } from 'react';
import { useGameState, PlayerType } from './GlobalGameState';

interface payload {
  // type: "init" | "terminating" | "acknowledged"
  event: "move" 
  contents?: string
  tile?: string;
}

type Props = {
  children?: any;
}

type WSType = {
  socket: WebSocket | null;
  emit: (key: string, value: any) => void;
}

export const WSContext = React.createContext<WSType>({
  socket: null,
  emit: () => {}
});



export const WSProvider = ({ children }: Props) => {
  const [socket, setSocket] = useState<WebSocket| null>(null);
  const [socketConnected, setSocketConnected] = useState<boolean>(false);
  
  const { setHexTileState, playerJoin, getPlayerInfo } = useGameState();
  
  useEffect(() => {
    const WS = new WebSocket(`ws://localhost:8080/ws/david`);
    WS.onopen = () => {
      setSocketConnected(true);
      playerJoin({uid: "abc", color: "red"});
    }
    WS.onmessage = (message) => {
      const payload:payload = JSON.parse(message.data) as payload
      console.log(payload);
      switch(payload.event) {
        case "move": {
          // set gamestate tile to be red
          const move = payload.tile;
          if(move) {
            
            // this is hardcoded for SINGLE PLAYER will need
            // backend player join broadcast to correctly work
            playerJoin({uid: "BOT", color: "green"});
            
            const playerColor = getPlayerInfo("BOT")?.color;
            if(!playerColor) break;
            setHexTileState(move, {
              user: "BOT",
              color: playerColor
            })
          }
          break;
        }
      }
    }
    setSocket(WS);
  },[])

  const emit = (key: string, data?: any) => {
    socket?.send(JSON.stringify({
      "event": key,
      "data": data
    }));
  }

  return (
    <WSContext.Provider
      value={{
        socket,
        emit,
      }}
    >
      {children}
    </WSContext.Provider>
  )
}

export const useSocket = () => useContext(WSContext);