import React from 'react'
import styled from 'styled-components';
import { Modal } from '@mui/material';


type Props = {
  open: boolean;
  handleClose: () => void;
}

const Container = styled.div`
  width: 800px;
  height: 550px;
  background: var(--accent-dark);
  color: white;

  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
`;

export default function SnapshotPopup({open, handleClose}: Props) {
  return (
    <Modal open={open} onClose={handleClose}>
      <Container>
        hello
      </Container>
    </Modal>
  )
}