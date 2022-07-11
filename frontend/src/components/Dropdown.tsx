import { Typography } from '@mui/material';
import React, { useEffect, useState } from 'react'
import styled from 'styled-components';

type Props = {}

// easier to create custom than try to style MUI dropdown
// to look like ours

const Container = styled.div`
  width: 80px;
  padding: 20px;
  background: var(--accent-darker);

`;

const DownArrowContainer = styled.div`

`

const mockSelections = [
  "elo",
  "mode",
]

export default function Dropdown({}: Props) {
  const [open, setOpen] = useState(false);

  useEffect(() => {
    return(() => {
      
    })
  },[])


  return (
    <>
      <Container onClick={()=>{setOpen(true)}}>
        <Typography>hi</Typography>
        <DownArrowContainer></DownArrowContainer>
      </Container>
      {open && 
        <DropdownMenuItems 
          selections={mockSelections}
        />
      }
    </>
  )
}


const DDMenuContainer = styled.div`
`;

const DDMenuItemContainer = styled.div`
  background: var(--accent-dark);
  width: 100px;
  padding: 10px;
  text-align: center;

  &:hover {
    background: var(--accent-darker);
  }
`;
function DropdownMenuItems({ selections }: { selections: string[] }) {

  return (
    <DDMenuContainer>
      {selections.map((selection: string) => (
        <DDMenuItemContainer>
          {selection}
        </DDMenuItemContainer>
      ))}
    </DDMenuContainer>
  )
}