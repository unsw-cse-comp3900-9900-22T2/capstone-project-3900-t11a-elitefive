import React, { useContext, useEffect, useState } from 'react';

const defaultBoard = [
  ['a1', 'a2', 'a3', 'a4', 'a5'],
  ['b1', 'b2', 'b3', 'b4', 'b5', 'b6'],
  ['c1', 'c2', 'c3', 'c4', 'c5', 'c6', 'c7'],
  ['d1', 'd2', 'd3', 'd4', 'd5', 'd6', 'd7', 'd8'],
  ['e1', 'e2', 'e3', 'e4', 'e5', 'e6', 'e7', 'e8', 'e9'],
  ['f1', 'f2', 'f3', 'f4', 'f5', 'f6', 'f7', 'f8'],
  ['g1', 'g2', 'g3', 'g4', 'g5', 'g6', 'g7'],
  ['h1', 'h2', 'h3', 'h4', 'h5', 'h6'],
  ['i1', 'i2', 'i3', 'i4', 'i5'],
]

type Props = {
  children?: any;
}

type GSType = {
  gamestate: GameStateType[][] | null;
  setHexTileState: (tile: string, tilestate: TileState) => void;
}

type TileState = {
  user: string | null;
  color: string | null;
}

export type GameStateType = {
  hexKey: string;
  hexState?: TileState;
}


export const GSContext = React.createContext<GSType>({
  gamestate: null,
  setHexTileState: () => {},
});

export const GSProvider = ({ children }: Props) => {
  const [GameState, SetGameState] = useState<GameStateType[][]>(
    defaultBoard.map((col: string[]) => col.map(
      (hexKey: string) => { 
        return {
          hexKey: hexKey,
          hexState: {} as TileState,
        }
      })
  ));

  const setHexTileState = (tile: string, tilestate: TileState) => {
    SetGameState(prev => prev.map((col: GameStateType[]) => col.map(
      (cell: GameStateType) => {
        if(cell.hexKey == tile) {
          return ({
            ...cell,
            hexState: tilestate
          })
        }
        return cell;
      }
    )))
  }

  return (
    <GSContext.Provider
      value={{
        gamestate: GameState,
        setHexTileState
      }}
    >
      {children}
    </GSContext.Provider>
  )
}

export const useGameState = () => useContext(GSContext);