import React, { useContext, useEffect, useState } from 'react';
import { useGameState, PlayerType } from './GlobalGameState';

interface payload {
  // type: "init" | "terminating" | "acknowledged"
  event: "move" | "moveconfirm" | "game_over"
  contents?: string;
  tile?: string;
  winner?: string;
}

type Props = {
  children?: any;
  gameId: string | undefined;
}

type WSType = {
  socket: WebSocket | null;
  emit: (key: string, value: any) => void;
}

export const WSContext = React.createContext<WSType>({
  socket: null,
  emit: () => {}
});



export const WSProvider = ({ children, gameId }: Props) => {
  const [socket, setSocket] = useState<WebSocket| null>(null);
  const [socketConnected, setSocketConnected] = useState<boolean>(false);
  
  const { playMove, setHexTileState, playerJoin, getPlayerInfo, setWinner } = useGameState();
  
  useEffect(() => {
    const WS = new WebSocket(`ws://localhost:8080/ws/game/${gameId}`);
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
            playerJoin({uid: "BOT", color: "blue"});
            const playerColor = getPlayerInfo("BOT")?.color;
            if(!playerColor) break;
            setHexTileState(move, {
              user: "BOT",
              color: playerColor
            })
          }
          break;
        }
        case "moveconfirm": {
          const payload:payload = JSON.parse(message.data) as payload
          const hexKey = payload.tile;
          if (hexKey) {
            playMove("abc", hexKey);
          }
          break;
        }
        case "game_over": {
          const payload:payload = JSON.parse(message.data) as payload
          const winner = payload.winner;
          if(winner) {
            setWinner(winner);
          }
          WS.close();
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