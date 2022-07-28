import React, { SetStateAction, useEffect, useState } from 'react';
import styled from 'styled-components';

// local
import HexCell from './HexCell';


type Props = {
  width?: number;
  height?: number;
  replayString: string[];
  setReplayString: React.Dispatch<SetStateAction<string[]>>
}

type RowStyleProps = {
  x_offset: number
}

type snapshotCellType = {
  position: string;
  color?: string;
}

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

const createSnapShotCells = () => {
  return defaultBoard.map((col: string[]) => col.map(
    (cell: string) => { 
      return {
        position: cell,
      }
  }))
}

const BoardContainer = styled.div`
  padding: 50px;
`;

// horizontal
const RowContainer = styled.span<RowStyleProps>`
  margin-left: ${props => props.x_offset}px;
  display: flex;
`;

// vertical
const ColumnContainer = styled.div`
  position: relative;
  margin-top: -15px;
`;

const DEFAULT_COLOR_1 = "#f33880";
const DEFAULT_COLOR_2 = "#1de254";


const DEFAULT_BOARD_WIDTH = 1200;
// const DEFAULT_BOARD_HEIGHT = 800;

export default function SnapshotBoard({ width, height, replayString, setReplayString }: Props) {

  const [state, setState] = useState<snapshotCellType[][]>(createSnapShotCells());


  // useEffect(() => {
  //   console.log(state);
  // },[state])

  const handleCellClick = (position:string, row_number:number) => () => {
      if(!replayString.find(replay_string => replay_string == position)) {

        const currColor = (replayString.length % 2 == 0) ? DEFAULT_COLOR_1 : DEFAULT_COLOR_2;

        // changes color of the clicked hexcell
        setState(prev => prev.map((row, index:number) => {
          if(index == row_number) {
            return row.map(cell => {
              if(cell.position == position) {
                return {
                  ...cell,
                  color: currColor,
                }
              }
              return cell;
            })
          } else {
            return row
          }
        }))

        // adds current cell to replayString
        setReplayString((prev:string[]) => [
          ...prev,
          position,
        ]);
      }
  }
  
  const renderRow = (row: snapshotCellType[] , row_number: number, x_offset: number) => {
    return (
      <RowContainer x_offset={x_offset}>
        {row.map(( { position, color } : snapshotCellType) => {
          return (
            <HexCell
              width={60}
              height={65}
              fill={color ? color : "var(--accent-darker)"}
              onClick={handleCellClick(position, row_number)}
            />
          )
        })}
      </RowContainer>
    )
  }

  const renderBoard = () => {
    const w = width ? width : DEFAULT_BOARD_WIDTH;

    return state.map((row, index) => {
      // it works
      const x_offset = Math.abs((index+1)- 5) * w/40
      const y_offset = index * 90;
      return (
        <ColumnContainer>
          {
            renderRow(row, index, x_offset)
          }
        </ColumnContainer>
      )
    })
  
  }

  return (
    <BoardContainer>
      {renderBoard()}
    </BoardContainer>
  )
}