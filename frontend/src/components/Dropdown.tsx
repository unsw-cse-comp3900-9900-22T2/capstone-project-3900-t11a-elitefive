import React, { useEffect, useRef, useState } from 'react'
import styled from 'styled-components';
import { Box, Typography } from '@mui/material';

import DownArrow from '../assets/DownArrow.svg';

type Props = {
  selected?: string;
  selections: string[];
  setSelected: (selection: string) => void;
}

// easier to create custom than try to style MUI dropdown
// to look like ours

const Container = styled.div`
  width: auto;
  background: var(--accent-dark);
  display: flex;
  cursor: pointer;
`;

const DownArrowContainer = styled.div`
  margin-left: auto;
  width: 30px;
  background: var(--accent-darker);
  display: flex;
  justify-content: center;
  align-items: center;
`

export default function Dropdown({ selected, selections, setSelected }: Props) {
  const [open, setOpen] = useState(false);
  const DropdownRef = useRef<HTMLElement>();


  /**
   * for closing dropdown menu when user clicks on anything that isnt the dropdown menu
   */

  // https://stackoverflow.com/questions/71193818/react-onclick-argument-of-type-eventtarget-is-not-assignable-to-parameter-of-t
  const handleGlobalClickListener = (event: MouseEvent) => {
    if(!DropdownRef || !DropdownRef.current) return;
    if(!DropdownRef.current.contains(event.target as Node)) {
      setOpen(false);
    }
  }

  useEffect(() => {
    window.addEventListener('click', handleGlobalClickListener)
    return(() => {
      window.removeEventListener('click', handleGlobalClickListener)
    })
  },[])

  return (
    <Box ref={DropdownRef} display="flex" flexDirection="column">
      <Container onClick={()=>{setOpen(prev => !prev)}}>
        <Box margin="5px 20px">
          <Typography textAlign="left">
            {selected ? selected : "choose"}
          </Typography>
        </Box>
        <DownArrowContainer><img src={DownArrow}/></DownArrowContainer>
      </Container>
      {open && 
        <DropdownMenuItems 
          selections={selections}
          setSelected={(selection:string) => {
            setOpen(false);
            setSelected(selection);
          }}
        />
      }
    </Box>
  )
}


const DDMenuContainer = styled.div`
  border: 0.1px solid grey;
`;

const DDMenuItemContainer = styled.div`

  position: relative;
  background: var(--accent-dark);
  padding: 10px;
  text-align: center;

  &:hover {
    background: var(--accent-darker);
  }
`;
function DropdownMenuItems({ selections, setSelected }: { selections: string[], setSelected: (selection: string) => void }) {

  return (
    <DDMenuContainer>
      {selections.map((selection: string) => (
        <DDMenuItemContainer onClick={() => {
          setSelected(selection);
        }}>
          {selection}
        </DDMenuItemContainer>
      ))}
    </DDMenuContainer>
  )
}